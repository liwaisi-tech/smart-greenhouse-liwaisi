idf_component_register(SRCS "actionPage.c"
                    INCLUDE_DIRS "include" 
                     REQUIRES "tempHumidity"
                    REQUIRES "yl69"
                    REQUIRES "sensorDHT")
