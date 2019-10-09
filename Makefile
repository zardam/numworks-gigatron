ifeq ($(PLATFORM),device)
SFLAGS += -DDEVICE
endif

apps += Gigatron::App
app_headers += apps/gigatron/app.h

app_src += $(addprefix apps/gigatron/,\
  app.cpp \
  gigatron_controller.cpp \
  gigatron_view.cpp \
)

app_images += apps/gigatron/gigatron_icon.png

i18n_files += $(addprefix apps/gigatron/,\
  base.de.i18n\
  base.en.i18n\
  base.es.i18n\
  base.fr.i18n\
  base.pt.i18n\
)

$(eval $(call depends_on_image,apps/gigatron/app.cpp,apps/gigatron/gigatron_icon.png))

ifneq ($(COMPILER),llvm)
$(BUILD_DIR)/apps/gigatron/app.o: SFLAGS+=-O3
endif
