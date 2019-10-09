#ifndef APPS_GIGATRON_CONTROLLER_H
#define APPS_GIGATRON_CONTROLLER_H

#include <escher.h>
#include "gigatron_view.h"

class AppsContainer;

namespace Gigatron {

class GigatronController : public ViewController {
  public:
    GigatronController();
    View * view() override;
  private:
    GigatronView m_view;
};

}

#endif
