class MathFunctions:

    def is_even(num: int) -> bool:
        return num & 1 == 0
    
    def power(self, base: int, exponent: int) -> int:
        if exponent == 0:
            return 1
        else:
            return base * self.power(base, exponent - 1)
        
    def is_palindrome(self, num: int) -> bool:
        original_num = num
        reversed_num = 0
        while num > 0:
            digit = num % 10
            reversed_num = reversed_num * 10 + digit
            num //= 10
        return original_num == reversed_num
        
    def fibonacci(self, n: int) -> int:
        if n <= 1:
            return n
        else:
            return self.fibonacci(n - 1) + self.fibonacci(n - 2)
    
    def is_armstrong_number(self, num: int) -> bool:
        original_num = num
        order = len(str(num))
        sum = 0
        while num > 0:
            digit = num % 10
            sum += digit ** order
            num //= 10
        return sum == original_num
    
    def is_prime(num: int) -> bool:
        if num <= 1:
            return False
        for i in range(2, num**0.5 + 1):
            if num % i == 0:
                return False
        return True
    
    def gcd(self, a: int, b: int) -> int:
        if b == 0:
            return a
        else:
            return self.gcd(b, a % b)
    
    def lcm(self, a: int, b: int) -> int:
        return (a * b) // self.gcd(a, b)

