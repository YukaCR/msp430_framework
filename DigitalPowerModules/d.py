def bin2hex(arr):
    k = 0
    for i in arr:
        assert(i == 1 or i == 0 )
        k <<= 1;k |= i
    return k
arr = [0,1,0,1,1,1,1]
print(bin2hex(arr))
assert(bin2hex(arr)==int('0b0101111',2))