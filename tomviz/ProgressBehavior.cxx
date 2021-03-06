/******************************************************************************

  This source file is part of the tomviz project.

  Copyright Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*****************************************************************************/
#include "ProgressBehavior.h"

#include "pqApplicationCore.h"
#include "pqProgressManager.h"

#include <QProgressDialog>

namespace tomviz {

ProgressBehavior::ProgressBehavior(QWidget* parentWindow)
  : Superclass(parentWindow), ProgressDialog(nullptr)
{
  pqProgressManager* progressManager =
    pqApplicationCore::instance()->getProgressManager();
  this->connect(progressManager, SIGNAL(enableProgress(bool)),
                SLOT(enableProgress(bool)));
  this->connect(progressManager, SIGNAL(progress(const QString&, int)),
                SLOT(progress(const QString, int)));
}

ProgressBehavior::~ProgressBehavior()
{
  if (this->ProgressDialog) {
    this->ProgressDialog->deleteLater();
  }
}

void ProgressBehavior::initialize()
{
  if (this->ProgressDialog) {
    return;
  }

  this->ProgressDialog = new QProgressDialog(
    "In progress...", "Cancel", 0, 100, qobject_cast<QWidget*>(this->parent()));
  this->ProgressDialog->setAutoClose(true);
  this->ProgressDialog->setAutoReset(false);
  this->ProgressDialog->setMinimumDuration(0); // 0 second.
}

void ProgressBehavior::enableProgress(bool enable)
{
  this->initialize();
  Q_ASSERT(this->ProgressDialog);

  if (enable) {
    this->ProgressDialog->setValue(0);
  } else {
    this->ProgressDialog->reset();
  }
}

void ProgressBehavior::progress(const QString& message, int progressAmount)
{
  this->initialize();
  Q_ASSERT(this->ProgressDialog);

  this->ProgressDialog->setLabelText(message);
  this->ProgressDialog->setValue(progressAmount);
}
}
