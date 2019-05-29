{
  'conditions': [

    ['OS=="mac"', {
      'variables': {
        # matches 10.9.X , 10.10 and outputs 10.9, 10.10, 10.11, 10.12, 10.13
        'OSX_VER%': "<!(sw_vers | grep 'ProductVersion:' | grep -o '10.[0-9]*')",
      }
    }, {
      'variables': {
        'OSX_VER%': "0",
      }
    }]
  ],
  "targets": [
    {
      "target_name": "im",
      "sources": [ "src/im.cc" ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "include_dirs" : [

      ],
      "conditions": [
        ['OSX_VER == "10.9" or OSX_VER == "10.10" or OSX_VER == "10.11" or OSX_VER == "10.12" or OSX_VER == "10.13"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '<!@(pkg-config --cflags ImageMagick++)'
            ],
            'OTHER_CPLUSPLUSFLAGS' : [
              '<!@(pkg-config --cflags ImageMagick++)',
              '-std=c++11',
              '-stdlib=libc++',
            ],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7', # -mmacosx-version-min=10.7
          },
          "libraries": [
             '<!@(pkg-config --libs ImageMagick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags ImageMagick++)'
          ],
        }],
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '<!@(pkg-config --cflags ImageMagick++)'
            ]
          },
          "libraries": [
             '<!@(pkg-config --libs ImageMagick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags ImageMagick++)'
          ],
        }],
        ['OS=="linux" or OS=="solaris" or OS=="freebsd"', { # not windows not mac
          "libraries": [
            '<!@(pkg-config --libs ImageMagick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags ImageMagick++)'
          ],
        }]
      ]
    }]
  }
