# A file is a class

class_name main     # Optional

extends SceneTree   # Inheritance

# Member variables
var str = "hello"
var arr = [1, 2, 3]
var dict = {"abc": 123, "111": "eee"}
var n: int
var v2 = Vector2(1, 2)
var v3 = Vector3(1, 2, 1)

# Constants
const NAME = "main"

# Enums
enum {OK, ERR}

# Constructor
func _init():
    hello()
    quit()

func hello():
    print(str)
    prints(PI, TAU, INF) # print with space
    printt(PI, TAU, INF) # print with tab

# Inner class
class Abc:
    pass
