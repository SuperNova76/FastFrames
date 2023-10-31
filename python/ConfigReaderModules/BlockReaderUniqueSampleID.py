from BlockReaderCommon import set_paths
set_paths()


from ConfigReaderCpp import ConfigReaderCppUniqueSampleID

class BlockReaderUniqueSampleID:
    def __init__(self, dsid : int, campaign : str, simulation : str):
        self.cpp_class = ConfigReaderCppUniqueSampleID(dsid, campaign, simulation)

    def get_ptr(self):
        return self.cpp_class.getPtr()

    def get_dsid(self):
        return self.cpp_class.getDsid()

    def get_campaign(self):
        return self.cpp_class.getCampaign()

    def get_simulation(self):
        return self.cpp_class.getSimulation()