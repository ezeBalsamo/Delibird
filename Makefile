SHARED_MODULE = Utils
MODULES = Team Broker GameCard GameBoy

.PHONY: $(SHARED_MODULE) $(MODULES)

all: $(SHARED_MODULE) $(MODULES)

utils:
	$(MAKE) -C $@

team:
	$(MAKE) -C $@

broker:
	$(MAKE) -C $@

gamecard:
	$(MAKE) -C $@

gameboy:
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SHARED_MODULE) clean
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean)