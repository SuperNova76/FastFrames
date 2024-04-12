"""
@file Source file of AutomaticRangeGenerator class.
"""

from copy import deepcopy
from BlockReaderCommon import set_paths
set_paths()

from python_wrapper.python.logger import Logger
from BlockOptionsGetter import BlockOptionsGetter

class AutomaticRangeGenerator:
    """
    AutomaticRangeGenerator class is used to generate a sequence of block based on the numbering sequence defined in the input block,
    for example unrolling one jet#N_pt into blocks for jet1_pt, jet2_pt, etc.
    """
    def __init__(self, list_of_replacement_sequences : list[dict]):
        """!Constructor of AutomaticRangeGenerator
        @param list_of_replacement_sequences: list[dict] - list of numbering sequences blocks from config
        """
        self.replace_sequences = deepcopy(list_of_replacement_sequences)

    def generate_range(self, input_block : dict, keys_to_replace : list[str]) -> list[dict]:
        """!Generate a range of blocks based on the numbering sequence defined in the input block
        @param input_block: dict - block contaning the replacement string, so for example jet#N_pt
        @param keys_to_replace: list[str] - list of keys to replace in the input block, so for example ["defintion", "name", "title"],
            it also supports nested blocks, where [["variation", "up"], ["variation", "down"], ["name"] will replace the corresponding values in subblocks
        @return list[dict]
        """
        updated_keys_to_replace = []
        for key in keys_to_replace:
            if type(key) == str:
                updated_keys_to_replace.append([key])
            else:
                updated_keys_to_replace.append(key)
        return self._generate_range(input_block, updated_keys_to_replace, self.replace_sequences)

    def _get_block_to_update(input_block : dict, keys_to_replace : list) -> dict:
        block_to_update = input_block
        for i_key in range(len(keys_to_replace)-1):
            key = keys_to_replace[i_key]
            if key in block_to_update:
                block_to_update = block_to_update[key]
            else:
                return None
        return block_to_update

    def _get_replacement_string_and_values(self, replacement_sequence : dict) -> tuple[str,list[str]]:
            this_replacement = BlockOptionsGetter(replacement_sequence)
            replacement_string = this_replacement.get("replacement_string", None, [str])
            squence_min = this_replacement.get("min", None, [int])
            squence_max = this_replacement.get("max", None, [int])
            sequence_values = this_replacement.get("values", None, [list], [str])
            unused_keys = this_replacement.get_unused_options()
            if len(unused_keys) > 0:
                raise ValueError("Unused keys in replacement sequence: {}".format(replacement_sequence))

            if replacement_string is None:
                raise ValueError("No replacement string specified in replacement sequence: {}".format(replacement_sequence))

            range_based = squence_min is not None and squence_max is not None
            value_based = sequence_values is not None

            if not (range_based ^ value_based):
                raise ValueError("Exactly one of min/max or values should be specified in replacement sequence: {}".format(replacement_sequence))
            elif range_based:
                return replacement_string,[str(index) for index in range(squence_min, squence_max + 1)]
            else:
                return replacement_string,sequence_values

    def _generate_range(self, input_block : dict, keys_to_replace : list[list[str]], replacement_sequences : list[dict]) -> list[dict]:
        if len(replacement_sequences) == 0:
            return [input_block]
        else:
            replacement_string, replaced_values = self._get_replacement_string_and_values(replacement_sequences[0])
            result = []
            for index in replaced_values:
                updated_block = deepcopy(input_block)
                for key_to_replace in keys_to_replace:
                    sub_block = AutomaticRangeGenerator._get_block_to_update(updated_block, key_to_replace)
                    if sub_block is None:
                        continue
                    key = key_to_replace[-1]
                    if key not in sub_block:
                        continue
                    sub_block[key] = sub_block[key].replace(replacement_string, index)
                result += self._generate_range(updated_block, keys_to_replace, replacement_sequences[1:])
            return result

    def unroll_sequence(list_of_block_dicts : list[dict], keys_to_update : list[str] = None) -> list[dict]:
        """!If numbering_sequence block is defined in a block, unroll the block into multiple blocks with different values of "keys_to_update" based on the numbering sequence
        @param list_of_block_dicts: list[dict]
        @param keys_to_update: list[str]
        @return list[dict]
        """
        result = []
        calculate_keys_to_update = keys_to_update is None
        list_of_block_dicts_copy = deepcopy(list_of_block_dicts)
        for block_dict in list_of_block_dicts_copy:
            if "numbering_sequence" not in block_dict:
                result.append(block_dict)
                continue
            replacement_sequences = block_dict["numbering_sequence"]
            if calculate_keys_to_update:
                keys_to_update = AutomaticRangeGenerator._get_list_of_keys_to_update(block_dict)
            if not isinstance(replacement_sequences, list):
                Logger.log_message("ERROR", "Replacement sequences should be a list. Maybe you forgot '-' ?")
                exit(1)
            del block_dict["numbering_sequence"]
            generator = AutomaticRangeGenerator(replacement_sequences)
            result += generator.generate_range(block_dict, keys_to_update)
        return result


    def _get_list_of_keys_to_update(block_dict : dict) -> list[list[str]]:
        if "numbering_sequence" not in block_dict:
            return []
        replacement_strings = []
        for replacement_sequence in block_dict["numbering_sequence"]:
            if not "replacement_string" in replacement_sequence:
                Logger.log_message("ERROR", "No replacement string specified in one of the numbering sequences")
                exit(1)
            replacement_strings.append(replacement_sequence["replacement_string"])

        def get_keys_to_update(block_dict : dict, replacement_strings : list[str], address : list[str]) -> list:
            keys_to_update = []
            for key, value in block_dict.items():
                if isinstance(value, str):
                    for replacement_string in replacement_strings:
                        if replacement_string in value:
                            keys_to_update.append(address + [key])
                if isinstance(value, dict):
                    keys_to_update = keys_to_update + get_keys_to_update(value, replacement_strings, address + [key])

            return keys_to_update

        return get_keys_to_update(block_dict, replacement_strings, [])