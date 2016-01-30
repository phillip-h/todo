all:
	cd src; $(MAKE)

debug:
	cd src; $(MAKE) debug

clean:
	@$(RM) todo

install:
	sudo mv todo /usr/bin/todo
	sudo cp completion/todoc /usr/share/bash-completion/completions/todo

remove:
	sudo rm /usr/bin/todo
	sudo rm /usr/share/bash-completion/completions/todo
