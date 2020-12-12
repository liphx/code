print(string.format("%x", 0xff & 0xabcd)) --> cd
print(string.format("%x", 0xff | 0xabcd)) --> abff
print(string.format("%x", 0xaaaa ~ -1)) --> ffffffffffff5555
print(string.format("%x", ~0)) --> ffffffffffffffff

print(0x7fffffffffffffff < 0x8000000000000000) --false
--unsigned less than
print(math.ult(0x7fffffffffffffff, 0x8000000000000000)) --true
mask = 0x8000000000000000
print((0x7fffffffffffffff ~ mask) < (0x8000000000000000 ~ mask)) -- true

s = string.pack("iii", 48, 49, 65)
print(s) --0   1   A
print(#s)
print(string.unpack("iii", s)) -->48      49      65      13
print(string.pack("bbb", 48, 49, 65)) --> 01A