all:
	cd src; $(MAKE)

debug:
	cd src; $(MAKE) debug

clean:
	@$(RM) todo

install:
	sudo mv todo /usr/local/bin/todo
	sudo cp completion/todoc /etc/bash_completion.d/todo

remove:
	sudo rm /usr/local/bin/todo
	sudo rm /etc/bash_completion.d/todo
