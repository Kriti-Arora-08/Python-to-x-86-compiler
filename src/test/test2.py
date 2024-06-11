class Fruit:
    def __init__(self, name_: str, vitamins: str):
        self.name = name_
        self.vitamins = vitamins
    def total_vitamins(self):
        total = 0
        for value in self.vitamins:
            total += ord(value)
        return total

apple = Fruit("Apple", "B12")
orange = Fruit("Orange", "B1")
banana = Fruit("Banana", "B4")
grapes = Fruit("Grapes", "B6")
mango = Fruit("Mango", "A")
gauva = Fruit("Gauva", "C")

richest_vitamin_fruit = None
richest_vitamin_amount = 0

apple_vitamins = apple.total_vitamins()
if apple_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = apple_vitamins
    richest_vitamin_fruit = apple

orange_vitamins = orange.total_vitamins()
if orange_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = orange_vitamins
    richest_vitamin_fruit = orange

banana_vitamins = banana.total_vitamins()
if banana_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = banana_vitamins
    richest_vitamin_fruit = banana

grapes_vitamins = grapes.total_vitamins()
if grapes_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = grapes_vitamins
    richest_vitamin_fruit = grapes

mango_vitamins = mango.total_vitamins()
if mango_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = mango_vitamins
    richest_vitamin_fruit = mango

gauva_vitamins = gauva.total_vitamins()
if gauva_vitamins > richest_vitamin_amount:
    richest_vitamin_amount = gauva_vitamins
    richest_vitamin_fruit = gauva

print("The fruit with the richest vitamin content is:")
print(richest_vitamin_fruit.name)

