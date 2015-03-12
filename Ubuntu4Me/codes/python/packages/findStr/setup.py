from distutils.core import setup
setup(
    name = 'findStr',
    version = '1.0.0',
    py_modules = ['findStr'],
    author = 'kavon ma',
    author_email = 'kavon.ma@alcatel-lucent.com',
    url = 'https://github.com/kavonm',
    description = 'find a specified string in a file',
    data_files = [('lib/python3.2/site-packages', ['data/data.txt'])]
)
