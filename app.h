#ifndef GIGATRON_APP_H
#define GIGATRON_APP_H

#include <escher.h>
#include "gigatron_controller.h"

class AppsContainer;

namespace Gigatron {

class App : public ::App {
public:
  class Descriptor : public ::App::Descriptor {
  public:
    I18n::Message name() override;
    I18n::Message upperName() override;
    const Image * icon() override;
  };
  class Snapshot : public ::App::Snapshot {
  public:
    Snapshot();
    App * unpack(Container * container) override;
    Descriptor * descriptor() override;
  };
  virtual void didBecomeActive(Window * window);
  virtual void willBecomeInactive();
private:
  App(Container * container, Snapshot * snapshot);
  GigatronController m_gigatronController;
  AppsContainer * m_appsContainer;
};

}

#endif
