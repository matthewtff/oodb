#include "oodb.hh"

#include <algorithm>
#include <functional>
#include <vector>

namespace oodb {

Db::Db (const std::string& FileName) : file_(FileName, File::ReadWrite) {
  Load();
}

Db::~Db () {
  Save();
}

void Db::Load() {
  LoadHashes();
  LoadSets();
  LoadStrings();
  LoadIntegers();
}

void Db::Save() {
  file_.Clear();
  SaveHashes();
  SaveSets();
  SaveStrings();
  SaveIntegers();
}

void Db::SetHash(const std::string& key, const HashValue& value) {
  hashes_[key][value.first] = value.second;
}

std::string& Db::GetHash(const std::string& key, const std::string& u_key) {
  return hashes_[key][u_key];
}

std::string Db::GetHash(const std::string& key,
                        const std::string& u_key) const {
  Hashes::const_iterator needed_hash = hashes_.find(key);
  if (needed_hash != hashes_.end()) {
    HashMap::const_iterator el = needed_hash->second.find(u_key);
    return el == needed_hash->second.end() ? std::string() : el->second;
  } else {
    return std::string();
  }
}


bool Db::AddSet(const std::string& key, const std::string& value) {
  return sets_[key].insert(value).second;
}

bool Db::CheckSet(const std::string& key, const std::string& value) const {
  Sets::const_iterator needed_set = sets_.find(key);
  return (needed_set != sets_.end()) ?
      needed_set->second.find(value) != needed_set->second.end() : false;
}

bool Db::RemoveSet(const std::string& key, const std::string& value) {
  return sets_[key].erase(value) > 0;
}


void Db::SetString(const std::string& key, const std::string& value) {
  strings_[key] = value;
}

std::string Db::GetString (const std::string& key) const {
  Strings::const_iterator needed_string = strings_.find(key);
  return needed_string != strings_.end() ?
      needed_string->second : std::string();
}

void Db::UnSetString(const std::string& key) {
  strings_.erase(key);
}

void Db::SetInteger(const std::string& key, Integer value) {
  integers_[key] = value;
}

Integer Db::GetInteger(const std::string& key) const {
  Integers::const_iterator needed_integer = integers_.find(key);
  return needed_integer != integers_.end() ? needed_integer->second : 0;
}

Integer Db::IncrementInteger(const std::string& key) {
  return ++integers_[key];
}

Integer Db::DecrementInteger(const std::string& key) {
  return --integers_[key];
}

void Db::UnSetInteger(const std::string& key) {
  integers_.erase(key);
}

void Db::Flush() {
  hashes_.clear();
  sets_.clear();
  strings_.clear();
  integers_.clear();
}

// private:

void Db::LoadHashes() {
  // Read number of hashes : [number]
  const uint32_t number_of_hashes = ReadNumber();
  for (uint32_t number = 0; number < number_of_hashes; ++number) {
    // Read hash global key : [size(key), key]
    const std::string global_key = ReadString();
    // Read number of key-value pairs : [number]
    const uint32_t number_of_pairs = ReadNumber();
    // Read key-value pairs in format [size(key), key, size(value), value]
    for (uint32_t key_value_pair = 0; key_value_pair < number_of_pairs;
         ++key_value_pair) {
      // Read key : [size(key), key]
      const std::string key = ReadString();
      // Read value : [size(value), value]
      const std::string value = ReadString();
      hashes_[global_key][key] = value;
    }
  }
}

void Db::LoadSets() {
  // Read number of sets : [number]
  const uint32_t number_of_sets = ReadNumber();
  for (uint32_t set = 0; set < number_of_sets; ++set) {
    // Read set global key : [size(key), key]
    const std::string global_key = ReadString();
    // Read number of set members : [number]
    const uint32_t number_of_members = ReadNumber();
    // Read members of set in format : [size(member), member]
    for (uint32_t member = 0; member < number_of_members; ++member) {
      // Read member : [size(member), member]
      const std::string value = ReadString();
      AddSet(global_key, value);
    }
  }
}

void Db::LoadStrings() {
  // Read number of strings : [number]
  const uint32_t number_of_strings = ReadNumber();
  for (uint32_t number = 0; number < number_of_strings; ++number) {
    // Read string global key : [size(key), key]
    const std::string global_key = ReadString();
    // Read string : [size(string), string]
    const std::string value = ReadString();
    SetString(global_key, value);
  }
}

void Db::LoadIntegers() {
  // Read number of integers : [number]
  uint32_t number_of_integers = ReadNumber();
  for (uint32_t number = 0; number < number_of_integers; ++number) {
    // Read integer global key : [size(key), key]
    const std::string global_key = ReadString();
    // Read integer : [integer]
    Integer integer;
    file_.Read(reinterpret_cast<void*>(&integer), sizeof(Integer));
    SetInteger(std::string(global_key), integer);
  }
}

void Db::SaveHashes() {
  // Write number of hashes : [number]
  WriteNumber(hashes_.size());
  for (const auto& hash : hashes_) {
    // Write hash global key : [size(key), key]
    WriteString(hash.first);
    // Write number of key-value pairs : [number]
    WriteNumber(hash.second.size());
    // Write key-value pairs in format [size(key), key, size(value), value]
    for (const auto& key_value : hash.second) {
      // Write key : [size(key), key]
      WriteString(key_value.first);
      // Write value : [size(value], value]
      WriteString(key_value.second);
    }
  }
}

void Db::SaveSets() {
  // Write number of sets : [number]
  WriteNumber(sets_.size());
  for (const auto& set : sets_) {
    // Write set global key : [size(key), key]
    WriteString(set.first);
    // Write number of set members : [number]
    WriteNumber(set.second.size());
    // Write members of set in format [size(member), member]
    std::for_each(set.second.begin(),
                  set.second.end(),
                  std::bind(&Db::WriteString, this, std::placeholders::_1));
  }
}

void Db::SaveStrings() {
  // Write number of strings : [number]
  WriteNumber(strings_.size());
  for (const auto& str : strings_) {
    // Write string global key : [size(key), key]
    WriteString(str.first);
    // Write string : [size(key), key]
    WriteString(str.second);
  }
}

void Db::SaveIntegers() {
  // Write number of integers : [number]
  WriteNumber(integers_.size());
  for (const auto& integer_pair : integers_) {
    // Write integer global key : [size(key), key]
    WriteString(integer_pair.first);
    // Write integer : [integer]
    file_.Write(reinterpret_cast<const void*>(&(integer_pair.second)),
                sizeof(Integer));
  }
}

uint32_t Db::ReadNumber() {
  uint32_t number = 0;
  file_.Read(reinterpret_cast<void*>(&number), sizeof(uint32_t));
  return number;
}

void Db::WriteNumber(const uint32_t number) {
  file_.Write(reinterpret_cast<const void*>(&number), sizeof(uint32_t));
}

std::string Db::ReadString() {
  const uint32_t size = ReadNumber();
  std::vector<char> key(size);
  file_.Read(reinterpret_cast<void*>(&key[0]), size);
  return std::string(key.begin(), key.end());
}

void Db::WriteString(const std::string& value) {
  const uint32_t size = value.size();
  WriteNumber(size);
  file_.Write(reinterpret_cast<const void*>(value.c_str()), size);
}

} // namespace oodb
