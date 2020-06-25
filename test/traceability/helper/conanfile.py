# Copyright @ 2019 Audi AG. All rights reserved.
# 
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
# 
# You may add additional accurate notices of copyright ownership.

from conans import ConanFile, tools
import os, sys

class TPCTRC_FEP_SDK_METAMODEL(ConanFile):

    path_trc = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), ".."))
    reqtify_project = "FEP.rqtf"
    options = {"generate_trc_plan": [True, False]}
    default_options = {"generate_trc_plan": False}
    
    _reqtifyRunHelper = None
    @property
    def reqtifyRunHelper(self):
        if not self._reqtifyRunHelper:
            with tools.pythonpath(self):
                # using 'exec' to avoid "Unresolved import" in PyDev-IDE
                exec('from reqtifyRunHelper import ReqtifyRunHelper')
                exec('self._reqtifyRunHelper = ReqtifyRunHelper(self)')
            # auto start reqtify project
            self._reqtifyRunHelper.additional_reqtify_options = os.path.join(self.path_trc, self.reqtify_project);
            self._reqtifyRunHelper.reqtify_license_path = "27420@reqtify-licca.in.audi.vwg;27420@reqtify1-licca.in.audi.vwg;27420@reqtify2-licca.in.audi.vwg"
        return self._reqtifyRunHelper
   
    def requirements(self):
        self.requires("enprove_tpctrc/ALL.3@enprove/stable")
     
    def build(self):
        # Replace path with path to test results
        Testresults_FEP = r"C:\06_FEP\FEP_SDK_GIT\FEP_METAMODEL\WIN10_x64_vc140_VS2017\test_results_dirs\fep_sdk_metamodel_tests"
    
        # generate starter bat
        self.reqtifyRunHelper.generate_reqtify_starter_bat(
            bat_dir = self.path_trc, 
            bat_name = "start_Reqtify.bat",
            additional_env_vars = {"FEP_SDK_METAMODEL_TEST_RESULTS":Testresults_FEP}) 

        self.reqtifyRunHelper.generate_report(
            ots_file = "generateTsrReport.ots",
            reqtify_project = os.path.join(self.path_trc, self.reqtify_project),
            reqtify_report_dir = self.path_trc,
            reqtify_report_name = "TSR",
            custom_report_vars={"REQTIFY_REPORT_TSR_CATEGORY":"Module", "FEP_SDK_METAMODEL_TEST_RESULTS":Testresults_FEP},
            disable_batch_mode = False)
