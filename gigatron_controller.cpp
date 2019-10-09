#include "gigatron_controller.h"

namespace Gigatron {

GigatronController::GigatronController() :
  ViewController(nullptr),
  m_view()
{
}

View * GigatronController::view() {
  return &m_view;
}

}
