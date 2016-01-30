DESTDIR = ""

all:
	cd src; $(MAKE)

debug:
	cd src; $(MAKE) debug

clean:
	@$(RM) todo

install:
	sudo mv todo $(DESTDIR)/usr/bin/todo
	sudo cp completion/todoc $(DESTDIR)/usr/share/bash-completion/completions/todo

remove:
	sudo rm $(DESTDIR)/usr/bin/todo
	sudo rm $(DESTDIR)/usr/share/bash-completion/completions/todo
