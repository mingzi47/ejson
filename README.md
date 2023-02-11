
## ejson

A recursive descent-based JSON interpreter and generator implemented using C++17.

## feature

- RAII
- C++17

## Usage

To generate documentation (you'll need `doxygen`; output will be in build/doc/):

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make doc
```
### Parser

`std::string data` already has JSON files 

```cpp
eee::Parser p;
p.parse(data) // or eee::Json json = p.parse(data);
```
`parse(std::string data)` return `Json`

or

```cpp
eee::Parser p{data};
```

You can use `getValue()` to get a `Json`

```cpp
std::Json json = p.getValue();
```

### Json

Initialize.

```cpp
eee::Json json;
// or eee::Json json = data;
eee::Json json{std::nullptr};
eee::Json json{bool};
eee::Json json{int};
eee::Json json{double};
eee::Json json{std::string};
eee::Json json{const char*};
eee::Json json{std::vector<Json>};
eee::Json json{std::map<std::string, Json>};
eee::Json json{eee::Json}
```

`Json` uses deep copy.

You can take out the value of a `std::optional`

```cpp
std::optional<std::string> value = eee::Json{std::string}.valueString();
```
About stringify

```cpp
std::string json = eee::Json{100}.stringify();

std::string json = eee::Json{100}.fmtStringify(); // formatted

std::cout << eee::Json{100}; // stringify(); 
```
You can use `=` directly to modify the values.

About array.

```cpp
eee::Json json{std::vector<eee::Json>{Json{5}}};

json[3] = eee::Json{100};

json.push_back{eee::Json{100}};

json.erase{std::size_t index};
```
About object.

```cpp
eee::Json json{std::map<std::string, eee::Json>{}};

json["key"] = eee::Json{100};

json.insert(std::make_pair("key", 100));

json.erase("key");

json.find("key") // return std::map<std::string, eee::Json>::iterator
```
