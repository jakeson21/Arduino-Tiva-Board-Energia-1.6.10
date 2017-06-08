# Arduino-Tiva-Board-Energia-1.6.10
Arduino Board Manager files for Arduino-Tiva

## Some things I learned along the way
When building a custom JSON index file for the Arduino Boards Manager, `<toolsDependencies.packager>` should match the top level `<packages.name>` value where the tools are described.
For instance when building the file for this project, I had to make packages.platforms.toolsDependencies.packager = packages.name in order to have arm-none-eabi-gcc and dslite installed along with this board.


    {
        "packages": [{
            ...
            "platforms": [{
                    "category": "ARDUINO-TIVA BOARDS",
                    "name": "Arduino-Tiva Boards",
                    ...
                    "toolsDependencies": [{
                            "packager":"Arduino-Tiva",
                            "name":"arm-none-eabi-gcc",
                            "version":"4.8.4-20140725"
                        },
                        {
                            "packager":"Arduino-Tiva",
                            "name":"dslite",
                            "version":"6.2.1.1594"
                        }
                    ],
                    ...
                }
            ],
            "tools": [{
                    "name":"arm-none-eabi-gcc",
                    "version":"4.8.4-20140725",
                    ...
                },
                {
                    "name":"dslite",
                    "version":"6.2.1.1594",
                    ...
                }
            ],
            "name": "Arduino-Tiva"
        }]
    }
