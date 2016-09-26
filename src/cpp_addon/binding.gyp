{
  "targets": [
    {
      "target_name": "binding",
      "sources":    [ "syntax.cc" ],
      "conditions": [
        ['OS=="mac"', {
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS" : [ "-std=c++11", "-stdlib=libc++" ],
            "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
            'GCC_ENABLE_CPP_RTTI': 'YES' 
          }
        }],
      ]
    }
  ]
}