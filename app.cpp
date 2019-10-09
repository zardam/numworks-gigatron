#include "app.h"
#include "../i18n.h"
#include "../apps_container.h"
#include "gigatron_icon.h"

extern "C" {
#include <assert.h>
#include "ROM.h"
}

#ifdef DEVICE
#include "../../ion/src/device/shared/drivers/display.h"
#endif

const uint8_t VSYNC = 0x80;
const uint8_t HSYNC = 0x40;

const uint8_t BUTTON_RIGHT = 0x01;
const uint8_t BUTTON_LEFT = 0x02;
const uint8_t BUTTON_DOWN = 0x04;
const uint8_t BUTTON_UP = 0x08;
const uint8_t BUTTON_START = 0x10;
const uint8_t BUTTON_SELECT = 0x20;
const uint8_t BUTTON_B = 0x40;
const uint8_t BUTTON_A = 0x80;

namespace Gigatron {

I18n::Message App::Descriptor::name() {
  return I18n::Message::Gigatron;
}

I18n::Message App::Descriptor::upperName() {
  return I18n::Message::GigatronCapital;
}

const Image * App::Descriptor::icon() {
  return ImageStore::GigatronIcon;
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

App * App::Snapshot::unpack(Container * container) {
  return new (container->currentAppBuffer()) App(container, this);
}

App::Snapshot::Snapshot() {
}

void App::didBecomeActive(Window * window) {
  ::App::didBecomeActive(window);

  KDColor palette[256];
  for(int i=0; i<256; i++) {
    palette[i] = KDColor::RGB888((i << 6) & 0xc0, (i << 4) & 0xc0, (i << 2) & 0xc0);
  }

  uint16_t PC = 0;
  uint8_t IR = 0, D = 0, AC = 0, X = 0, Y = 0, OUT = 0, RAM[1<<15], undef = 0, IN = 0xff, falling, prev_out = 0;
  int x = 0, y = 0;

  for (;;) {
    uint16_t CPC = PC++;

    switch(IR) {
      // LOAD
      case 0x00:
        AC = D;
        break;
      case 0x01:
        AC = RAM[D];
        break;
      case 0x02:
        break;
      case 0x03:
        AC = IN;
        break;
      case 0x04:
        AC = D;
        break;
      case 0x05:
        AC = RAM[X];
        break;
      case 0x06:
        break;
      case 0x07:
        AC = IN;
        break;
      case 0x08:
        AC = D;
        break;
      case 0x09:
        AC = RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0x0a:
        break;
      case 0x0b:
        AC = IN;
        break;
      case 0x0c:
        AC = D;
        break;
      case 0x0d:
        AC = RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0x0e:
        break;
      case 0x0f:
        AC = IN;
        break;
      case 0x10:
        X = D;
        break;
      case 0x11:
        X = RAM[D];
        break;
      case 0x12:
        X = AC;
        break;
      case 0x13:
        X = IN;
        break;
      case 0x14:
        Y = D;
        break;
      case 0x15:
        Y = RAM[D];
        break;
      case 0x16:
        Y = AC;
        break;
      case 0x17:
        Y = IN;
        break;
      case 0x18:
        OUT = D;
        break;
      case 0x19:
        OUT = RAM[D];
        break;
      case 0x1a:
        OUT = AC;
        break;
      case 0x1b:
        OUT = IN;
        break;
      case 0x1c:
        OUT = D;
        break;
      case 0x1d:
        OUT = RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0x1e:
        OUT = AC;
        break;
      case 0x1f:
        OUT = IN;
        break;

      // AND
      case 0x20:
        AC &= D;
        break;
      case 0x21:
        AC &= RAM[D];
        break;
      case 0x22:
        break;
      case 0x23:
        AC &= IN;
        break;
      case 0x24:
        AC &= D;
        break;
      case 0x25:
        AC &= RAM[X];
        break;
      case 0x26:
        break;
      case 0x27:
        AC &= IN;
        break;
      case 0x28:
        AC &= D;
        break;
      case 0x29:
        AC &= RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0x2a:
        break;
      case 0x2b:
        AC &= IN;
        break;
      case 0x2c:
        AC &= D;
        break;
      case 0x2d:
        AC &= RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0x2e:
        break;
      case 0x2f:
        AC &= IN;
        break;
      case 0x30:
        X = AC & D;
        break;
      case 0x31:
        X = AC & RAM[D];
        break;
      case 0x32:
        X = AC;
        break;
      case 0x33:
        X = AC & IN;
        break;
      case 0x34:
        Y = AC & D;
        break;
      case 0x35:
        Y = AC & RAM[D];
        break;
      case 0x36:
        Y = AC;
        break;
      case 0x37:
        Y = AC & IN;
        break;
      case 0x38:
        OUT = AC & D;
        break;
      case 0x39:
        OUT = AC & RAM[D];
        break;
      case 0x3a:
        OUT = AC;
        break;
      case 0x3b:
        OUT = AC & IN;
        break;
      case 0x3c:
        OUT = AC & D;
        break;
      case 0x3d:
        OUT = AC & RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0x3e:
        OUT = AC;
        break;
      case 0x3f:
        OUT = AC & IN;
        break;

      // OR
      case 0x40:
        AC |= D;
        break;
      case 0x41:
        AC |= RAM[D];
        break;
      case 0x42:
        break;
      case 0x43:
        AC |= IN;
        break;
      case 0x44:
        AC |= D;
        break;
      case 0x45:
        AC |= RAM[X];
        break;
      case 0x46:
        break;
      case 0x47:
        AC |= IN;
        break;
      case 0x48:
        AC |= D;
        break;
      case 0x49:
        AC |= RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0x4a:
        break;
      case 0x4b:
        AC |= IN;
        break;
      case 0x4c:
        AC |= D;
        break;
      case 0x4d:
        AC |= RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0x4e:
        break;
      case 0x4f:
        AC |= IN;
        break;
      case 0x50:
        X = AC | D;
        break;
      case 0x51:
        X = AC | RAM[D];
        break;
      case 0x52:
        X = AC;
        break;
      case 0x53:
        X = AC | IN;
        break;
      case 0x54:
        Y = AC | D;
        break;
      case 0x55:
        Y = AC | RAM[D];
        break;
      case 0x56:
        Y = AC;
        break;
      case 0x57:
        Y = AC | IN;
        break;
      case 0x58:
        OUT = AC | D;
        break;
      case 0x59:
        OUT = AC | RAM[D];
        break;
      case 0x5a:
        OUT = AC;
        break;
      case 0x5b:
        OUT = AC | IN;
        break;
      case 0x5c:
        OUT = AC | D;
        break;
      case 0x5d:
        OUT = AC | RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0x5e:
        OUT = AC;
        break;
      case 0x5f:
        OUT = AC | IN;
        break;

      // XOR
      case 0x60:
        AC ^= D;
        break;
      case 0x61:
        AC ^= RAM[D];
        break;
      case 0x62:
        AC = 0;
        break;
      case 0x63:
        AC ^= IN;
        break;
      case 0x64:
        AC ^= D;
        break;
      case 0x65:
        AC ^= RAM[X];
        break;
      case 0x66:
        AC = 0;
        break;
      case 0x67:
        AC ^= IN;
        break;
      case 0x68:
        AC ^= D;
        break;
      case 0x69:
        AC ^= RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0x6a:
        AC = 0;
        break;
      case 0x6b:
        AC ^= IN;
        break;
      case 0x6c:
        AC ^= D;
        break;
      case 0x6d:
        AC ^= RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0x6e:
        AC = 0;
        break;
      case 0x6f:
        AC ^= IN;
        break;
      case 0x70:
        X = AC ^ D;
        break;
      case 0x71:
        X = AC ^ RAM[D];
        break;
      case 0x72:
        X = 0;
        break;
      case 0x73:
        X = AC ^ IN;
        break;
      case 0x74:
        Y = AC ^ D;
        break;
      case 0x75:
        Y = AC ^ RAM[D];
        break;
      case 0x76:
        Y = 0;
        break;
      case 0x77:
        Y = AC ^ IN;
        break;
      case 0x78:
        OUT = AC ^ D;
        break;
      case 0x79:
        OUT = AC ^ RAM[D];
        break;
      case 0x7a:
        OUT = 0;
        break;
      case 0x7b:
        OUT = AC ^ IN;
        break;
      case 0x7c:
        OUT = AC ^ D;
        break;
      case 0x7d:
        OUT = AC ^ RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0x7e:
        OUT = 0;
        break;
      case 0x7f:
        OUT = AC ^ IN;
        break;

      // ADD
      case 0x80:
        AC += D;
        break;
      case 0x81:
        AC += RAM[D];
        break;
      case 0x82:
        AC += AC;
        break;
      case 0x83:
        AC += IN;
        break;
      case 0x84:
        AC += D;
        break;
      case 0x85:
        AC += RAM[X];
        break;
      case 0x86:
        AC += AC;
        break;
      case 0x87:
        AC += IN;
        break;
      case 0x88:
        AC += D;
        break;
      case 0x89:
        AC += RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0x8a:
        AC += AC;
        break;
      case 0x8b:
        AC += IN;
        break;
      case 0x8c:
        AC += D;
        break;
      case 0x8d:
        AC += RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0x8e:
        AC += AC;
        break;
      case 0x8f:
        AC += IN;
        break;
      case 0x90:
        X = AC + D;
        break;
      case 0x91:
        X = AC + RAM[D];
        break;
      case 0x92:
        X = AC + AC;
        break;
      case 0x93:
        X = AC + IN;
        break;
      case 0x94:
        Y = AC + D;
        break;
      case 0x95:
        Y = AC + RAM[D];
        break;
      case 0x96:
        Y = AC + AC;
        break;
      case 0x97:
        Y = AC + IN;
        break;
      case 0x98:
        OUT = AC + D;
        break;
      case 0x99:
        OUT = AC + RAM[D];
        break;
      case 0x9a:
        OUT = AC + AC;
        break;
      case 0x9b:
        OUT = AC + IN;
        break;
      case 0x9c:
        OUT = AC + D;
        break;
      case 0x9d:
        OUT = AC + RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0x9e:
        OUT = AC + AC;
        break;
      case 0x9f:
        OUT = AC + IN;
        break;

      // SUB
      case 0xa0:
        AC -= D;
        break;
      case 0xa1:
        AC -= RAM[D];
        break;
      case 0xa2:
        AC = 0;
        break;
      case 0xa3:
        AC -= IN;
        break;
      case 0xa4:
        AC -= D;
        break;
      case 0xa5:
        AC -= RAM[X];
        break;
      case 0xa6:
        AC = 0;
        break;
      case 0xa7:
        AC -= IN;
        break;
      case 0xa8:
        AC -= D;
        break;
      case 0xa9:
        AC -= RAM[((Y << 8) | D) & 0x7fff];
        break;
      case 0xaa:
        AC = 0;
        break;
      case 0xab:
        AC -= IN;
        break;
      case 0xac:
        AC -= D;
        break;
      case 0xad:
        AC -= RAM[((Y << 8) | X) & 0x7fff];
        break;
      case 0xae:
        AC = 0;
        break;
      case 0xaf:
        AC -= IN;
        break;
      case 0xb0:
        X = AC - D;
        break;
      case 0xb1:
        X = AC - RAM[D];
        break;
      case 0xb2:
        X = 0;
        break;
      case 0xb3:
        X = AC - IN;
        break;
      case 0xb4:
        Y = AC - D;
        break;
      case 0xb5:
        Y = AC - RAM[D];
        break;
      case 0xb6:
        Y = 0;
        break;
      case 0xb7:
        Y = AC - IN;
        break;
      case 0xb8:
        OUT = AC - D;
        break;
      case 0xb9:
        OUT = AC - RAM[D];
        break;
      case 0xba:
        OUT = 0;
        break;
      case 0xbb:
        OUT = AC - IN;
        break;
      case 0xbc:
        OUT = AC - D;
        break;
      case 0xbd:
        OUT = AC - RAM[((Y << 8) | X++) & 0x7fff];
        break;
      case 0xbe:
        OUT = 0;
        break;
      case 0xbf:
        OUT = AC - IN;
        break;

      // STORE
      case 0xc0:
        RAM[D] = D;
        break;
      case 0xc1:
        RAM[D] = undef;
        break;
      case 0xc2:
        RAM[D] = AC;
        break;
      case 0xc3:
        RAM[D] = IN;
        break;
      case 0xc4:
        RAM[X] = D;
        break;
      case 0xc5:
        RAM[X] = undef;
        break;
      case 0xc6:
        RAM[X] = AC;
        break;
      case 0xc7:
        RAM[X] = IN;
        break;
      case 0xc8:
        RAM[((Y << 8) | D) & 0x7fff] = D;
        break;
      case 0xc9:
        RAM[((Y << 8) | D) & 0x7fff] = undef;
        break;
      case 0xca:
        RAM[((Y << 8) | D) & 0x7fff] = AC;
        break;
      case 0xcb:
        RAM[((Y << 8) | D) & 0x7fff] = IN;
        break;
      case 0xcc:
        RAM[((Y << 8) | X) & 0x7fff] = D;
        break;
      case 0xcd:
        RAM[((Y << 8) | X) & 0x7fff] = undef;
        break;
      case 0xce:
        RAM[((Y << 8) | X) & 0x7fff] = AC;
        break;
      case 0xcf:
        RAM[((Y << 8) | X) & 0x7fff] = IN;
        break;
      case 0xd0:
        RAM[D] = D;
        X = D;
        break;
      case 0xd1:
        RAM[D] = undef;
        X = undef;
        break;
      case 0xd2:
        RAM[D] = AC;
        X = AC;
        break;
      case 0xd3:
        RAM[D] = IN;
        X = IN;
        break;
      case 0xd4:
        RAM[D] = D;
        Y = D;
        break;
      case 0xd5:
        RAM[D] = undef;
        Y = undef;
        break;
      case 0xd6:
        RAM[D] = AC;
        Y = AC;
        break;
      case 0xd7:
        RAM[D] = IN;
        Y = IN;
        break;
      case 0xd8:
        RAM[D] = D;
        break;
      case 0xd9:
        RAM[D] = undef;
        break;
      case 0xda:
        RAM[D] = AC;
        break;
      case 0xdb:
        RAM[D] = IN;
        break;
      case 0xdc:
        RAM[((Y << 8) | X++) & 0x7fff] = D;
        break;
      case 0xdd:
        RAM[((Y << 8) | X++) & 0x7fff] = undef;
        break;
      case 0xde:
        RAM[((Y << 8) | X++) & 0x7fff] = AC;
        break;
      case 0xdf:
        RAM[((Y << 8) | X++) & 0x7fff] = IN;
        break;

      // BRANCH
      case 0xe0:
        PC = (Y << 8) | D;
        break;
      case 0xe1:
        PC = (Y << 8) | RAM[D];
        break;
      case 0xe2:
        PC = (Y << 8) | AC;
        break;
      case 0xe3:
        PC = (Y << 8) | IN;
        break;
      case 0xe4:
        if(((int8_t) AC) > 0) PC = (PC & 0xff00) | D;
        break;
      case 0xe5:
        if(((int8_t) AC) > 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xe6:
        if(((int8_t) AC) > 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xe7:
        if(((int8_t) AC) > 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xe8:
        if(((int8_t) AC) < 0) PC = (PC & 0xff00) | D;
        break;
      case 0xe9:
        if(((int8_t) AC) < 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xea:
        if(((int8_t) AC) < 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xeb:
        if(((int8_t) AC) < 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xec:
        if(((int8_t) AC) != 0) PC = (PC & 0xff00) | D;
        break;
      case 0xed:
        if(((int8_t) AC) != 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xee:
        if(((int8_t) AC) != 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xef:
        if(((int8_t) AC) != 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xf0:
        if(((int8_t) AC) == 0) PC = (PC & 0xff00) | D;
        break;
      case 0xf1:
        if(((int8_t) AC) == 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xf2:
        if(((int8_t) AC) == 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xf3:
        if(((int8_t) AC) == 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xf4:
        if(((int8_t) AC) >= 0) PC = (PC & 0xff00) | D;
        break;
      case 0xf5:
        if(((int8_t) AC) >= 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xf6:
        if(((int8_t) AC) >= 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xf7:
        if(((int8_t) AC) >= 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xf8:
        if(((int8_t) AC) <= 0) PC = (PC & 0xff00) | D;
        break;
      case 0xf9:
        if(((int8_t) AC) <= 0) PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xfa:
        if(((int8_t) AC) <= 0) PC = (PC & 0xff00) | AC;
        break;
      case 0xfb:
        if(((int8_t) AC) <= 0) PC = (PC & 0xff00) | IN;
        break;
      case 0xfc:
        PC = (PC & 0xff00) | D;
        break;
      case 0xfd:
        PC = (PC & 0xff00) | RAM[D];
        break;
      case 0xfe:
        PC = (PC & 0xff00) | AC;
        break;
      case 0xff:
        PC = (PC & 0xff00) | IN;
        break;
    }

    IR = ROM[CPC<<1];
    D = ROM[(CPC<<1) + 1];

    falling = prev_out & ~OUT;
    prev_out = OUT;

    if (falling & VSYNC) {
      x = 0;
      y = 0;

      IN = 0;
      uint64_t k = Ion::Keyboard::scan();
      if(k & 0x01) IN |= BUTTON_LEFT;
      if(k & 0x02) IN |= BUTTON_UP;
      if(k & 0x04) IN |= BUTTON_DOWN;
      if(k & 0x08) IN |= BUTTON_RIGHT;
      if(k & 0x10) IN |= BUTTON_B;
      if(k & 0x20) IN |= BUTTON_A;
      if(k & 0x40) IN |= BUTTON_SELECT;
      if(k & 0x80) IN |= BUTTON_START;
      if(k & 0x100) IN |= BUTTON_START;
      if(k & 0xfffffffffffffe00) break;
      IN ^= 0xff;

      #ifdef DEVICE
      Ion::Device::Display::setDrawingArea(KDRect(0, 0, 320, 240), Ion::Device::Display::Orientation::Landscape);
      *Ion::Device::Display::CommandAddress = Ion::Device::Display::Command::MemoryWrite;
      #endif

      undef = Ion::random() & 0xff;
    }

    if (falling & HSYNC) {
      x = 0;
      y++;
    }

    if ((OUT & (VSYNC | HSYNC)) != (VSYNC | HSYNC)) {
       continue;
    }

    if ((y >= 34 && y < 480+34) && (x >= 12 && x < 160+12) && y%2 == 0) {
      #ifdef DEVICE
      *Ion::Device::Display::DataAddress = palette[OUT];
      *Ion::Device::Display::DataAddress = palette[OUT];
      #else
      Ion::Display::pushRectUniform(KDRect((x-12)*2, (y-34)/2, 2, 1), palette[OUT]);
      #endif
    }

    x++;
  }

  m_appsContainer->switchTo(m_appsContainer->appSnapshotAtIndex(0));
}

void App::willBecomeInactive() {
  ::App::willBecomeInactive();
}

App::App(Container * container, Snapshot * snapshot) :
  ::App(snapshot, &m_gigatronController),
  m_appsContainer((AppsContainer*) container)
{
}

}
