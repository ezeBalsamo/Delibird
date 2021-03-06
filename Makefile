SHARED_MODULE = Utils
MODULES = Team Broker Gamecard Gameboy Config_updater

.PHONY: $(SHARED_MODULE) $(MODULES)

all: $(SHARED_MODULE) $(MODULES)

Utils:
	$(MAKE) -C $@

Team: Utils
	$(MAKE) -C $@

Broker: Utils
	$(MAKE) -C $@

Gamecard: Utils
	$(MAKE) -C $@

Gameboy: Utils
	$(MAKE) -C $@

Config_updater: Utils
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SHARED_MODULE) clean
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean &&) true