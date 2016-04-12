MANAGE=venv/bin/python ./manage.py

all: venv
	@pep8 *.py

run:
	./weather.py --place Helsinki,fi London,uk

venv: venv/bin/activate
venv/bin/activate: requirements.txt
	test -d venv || virtualenv venv
	venv/bin/pip install -Ur requirements.txt
	touch venv/bin/activate
