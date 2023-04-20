https://gist.github.com/tomtung/2922865

松本行弘

解释型、面向对象、动态类型

Rails

`irb`

```ruby
#!/usr/bin/env ruby

puts 'hello, world.'
user = 'liph'
puts "hello, #{user}"

x = 4
puts 'true' if x == 4
puts 'true' unless x == 4

if x == 4
    puts 'true'
end

10.times {puts 'liph'}

i = 1
while i <= 10
    puts "This is sentence number #{i}"
      i += 1
end

(1..10).each {|i| puts "This is sentence number #{i}"}

'Hello, Ruby'.index('Ruby')

a = [0, 1, 2]
a[0..1]
a.push(4)
a.pop()

numbers = {1=>"a", 2=>"b"}
numbers[1]

a = {:a=>1, :b=>2}
```