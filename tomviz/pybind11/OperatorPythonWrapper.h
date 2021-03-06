/******************************************************************************

  This source file is part of the tomviz project.


  Copyright Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/
#ifndef tomvizOperatorPythonWrapper_h
#define tomvizOperatorPythonWrapper_h

#include <string>

namespace tomviz {
class OperatorPython;
}

struct OperatorPythonWrapper
{
  OperatorPythonWrapper(void* o);
  bool canceled();
  void setTotalProgressSteps(int progress);
  int totalProgressSteps();
  void setProgressStep(int progress);
  int progressStep();
  void setProgressMessage(const std::string& message);
  std::string progressMessage();

  tomviz::OperatorPython* op = nullptr;
};

#endif
