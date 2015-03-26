#ifndef oodb_hh
#define oodb_hh

#include <string>
#include <utility>

#include <unordered_map>
#include <unordered_set>
#include <cstdint>

#include "file_driver.hh"

namespace oodb {

typedef std::unordered_map<std::string, std::string> HashMap;
typedef std::unordered_set<std::string> Set;
typedef uint64_t Integer;

typedef std::unordered_map<std::string, HashMap> Hashes;
typedef std::unordered_map<std::string, Set> Sets;
typedef std::unordered_map<std::string, std::string> Strings;
typedef std::unordered_map<std::string, Integer> Integers;

typedef std::pair<std::string, std::string> HashValue;

class Db {
 public:
  Db(const std::string& FileName);
  ~Db();
  void Load();
  void Save();

  HashMap& GetHashMap(const std::string& key) const;
  void SetHash(const std::string& key, const HashValue& value);
  std::string& GetHash(const std::string& key, const std::string& u_key);
  std::string GetHash(const std::string& key, const std::string& u_key) const;

  Set& GetSet(const std::string& key) { return sets_[key]; }
  bool AddSet(const std::string& key, const std::string& value);
  bool CheckSet(const std::string& key, const std::string& value) const;
  bool RemoveSet(const std::string& key, const std::string& value);

  void SetString(const std::string& key, const std::string& value);
  std::string GetString(const std::string& key) const;
  void UnSetString(const std::string& key);

  void SetInteger(const std::string& key, Integer value);
  Integer GetInteger(const std::string& key) const;
  Integer IncrementInteger(const std::string& key);
  Integer DecrementInteger(const std::string& key);
  void UnSetInteger(const std::string& key);

  void Flush();

  Hashes& GetHashes() { return hashes_; }
  Sets& GetSets() { return sets_; }
  Strings& GetStrings() { return strings_; }
  Integers& GetIntegers() { return integers_; }

private:

  void LoadHashes ();
  void LoadSets ();
  void LoadStrings ();
  void LoadIntegers ();

  void SaveHashes ();
  void SaveSets ();
  void SaveStrings ();
  void SaveIntegers ();

  uint32_t ReadNumber();
  std::string ReadString();
  void WriteNumber(const uint32_t number);
  void WriteString(const std::string& value);

private:
  File file_;

  Hashes hashes_;
  Sets sets_;
  Strings strings_;
  Integers integers_;
};

} // namespace oodb

#endif // oodb_hh
