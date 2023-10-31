from BlockReaderCommon import set_paths
set_paths()


from ConfigReaderCpp import ConfigReaderCppUniqueSampleID

class BlockReaderUniqueSampleID:
    def __init__(self, dsid : int, campaign : str, simulation : str):
        self.cpp_unique_sample_id = ConfigReaderCppUniqueSampleID(dsid, campaign, simulation)

    def get_ptr(self):
        return self.cpp_unique_sample_id.getPtr()

    def get_dsid(self):
        return self.cpp_unique_sample_id.getDsid()

    def get_campaign(self):
        return self.cpp_unique_sample_id.getCampaign()

    def get_simulation(self):
        return self.cpp_unique_sample_id.getSimulation()