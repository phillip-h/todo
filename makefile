DESTDIR = ""

all:
	cd src; $(MAKE)

debug:
	cd src; $(MAKE) debug

clean:
	@$(RM) todo

install:
	install -D todo $(DESTDIR)/usr/bin/todo
	install -D completion/todoc $(DESTDIR)/usr/share/bash-completion/completions/todo

remove:
	sudo rm $(DESTDIR)/usr/bin/todo
	sudo rm $(DESTDIR)/usr/share/bash-completion/completions/todo
