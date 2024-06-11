def fib(i:int, a:int, b:int, c:int):
  if i==1:
    print("\nfrom ")
    print(a)
    print(" to ")
    print(b)
  else:
    fib(i-1, a, c, b)
    print("\nfrom ")
    print(a)
    print(" to ")
    print(b)
    fib(i-1, c, b, a)
  return 

def main():
  print("Finishing Where We Started:\n")
  fib(4, 1, 2, 3)
  print("\n\nDONE\n")
