from re import search
from os.path import join, abspath

if __name__ == '__main__':
    with open(abspath(join('src', 'common', 'version.cpp'))) as version:
        text = version.read()
        VERSION_RE = r'const u16 TYPE = \b(.*)\b;'

        major = None
        minor = None
        patch = None

        for t in ['major', 'minor', 'patch']:
            exec('{} = {}'.format(t, search(VERSION_RE.replace("TYPE", t), text).group(1)))

        print('{}.{}.{}'.format(major, minor, patch))