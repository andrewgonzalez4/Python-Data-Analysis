import json
import numpy
# with open('LCDtest.json', 'r') as f:
#     lcdjson = json.load(f)
lcdjson = [{"PID": True,
            "Motors":{
                "Motor 1": {
                    "Speed":[10, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 2": {
                    "Speed":[10, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 3": {
                    "Speed":[10, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 4": {
                    "Speed":[10, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                }
            },
            "Sensors": True}]


SpeedList_mtr_1 = []
PosList_mtr_1 = []
for files in lcdjson:
    SpeedList_mtr_1 = files['Motors']['Motor 1']['Speed']
    PosList_mtr_1 = files['Motors']['Motor 1']['Position']
print(SpeedList_mtr_1)