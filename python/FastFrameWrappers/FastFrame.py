import os
import sys

this_dir = "/".join(os.path.dirname(os.path.abspath(__file__)).split("/")[0:-1])
print(this_dir)
sys.path.append(this_dir)

from ConfigReaderModules.BlockReaderCommon import set_paths
set_paths()

from ConfigReaderCpp import ConfigReaderCppGeneral, MainFrame
from python_wrapper.python.logger import Logger



if __name__ == "__main__":
    main_frame = MainFrame()
    main_frame.init()
    main_frame.executeHistograms()