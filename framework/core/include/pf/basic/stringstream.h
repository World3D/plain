/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id stringstream.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2017/04/27 20:21
 * @uses Like the <sstream>, 
 *       but this class will read/write by string buffer directly.
*/
#ifndef PF_BASIC_STRINGSTREAM_H_
#define PF_BASIC_STRINGSTREAM_H_

#include "pf/basic/config.h"
#include "pf/sys/assert.h"

namespace pf_basic {

class stringstream {

 public:
   stringstream(char *str, size_t size) :
     str_{str}, size_{size}, cur_{0} {}
   ~stringstream() {}

 public:
   void read(char *var, size_t size) {
     Assert(str_ != nullptr && size_ > 0);
     if (size + cur_ > size_) return;
     memcpy(var, str_ + cur_, size);
     cur_ += size;
   }
   void write(const char *var, size_t size) {
     Assert(str_ != nullptr && size_ > 0);
     if (size + cur_ > size_) return;
     memcpy(str_ + cur_, var, size);
     cur_ += size;
   }

 public:
   void clear() {
     cur_ = 0;
     memset(str_, 0, size_);
   }

   bool full() const {
     return cur_ >= size_;
   }

   size_t get_position() const {
     return cur_;
   }

   void set_position(size_t position) {
     Assert(position >= 0 && position < size_);
     if (position < 0 || position >= size_) return;
     cur_ = position;
   }

 public:
   template <typename T>
   stringstream &operator >> (T &var) {
     read((char *)&var, sizeof(var));
     return *this;
   }

   stringstream &operator >> (std::string &var) {
     char temp[SSTREAM_STRING_SIZE_MAX]{0};
     int32_t size{0};
     read((char *)&size, sizeof(size));
     if (size > 0) read(temp, size);
     var = temp;
     return *this;
   }

   stringstream &operator >> (char *var) {
     int32_t size{0};
     read((char *)&size, sizeof(size));
     if (size > 0) read(var, size);
     return *this;
   }

 public:
   template <typename T>
   stringstream &operator << (T var) {
     write((char *)&var, sizeof(var));
     return *this;
   }

   stringstream &operator << (const std::string var) {
     auto size = var.size();
     write((char *)&size, sizeof(int32_t));
     write(var.c_str(), size);
     return *this;
   }

   stringstream &operator << (const char *var) {
     auto size = strlen(var);
     write((char *)&size, sizeof(int32_t));
     write(var, size);
     return *this;
   }

 private:
   char *str_;
   size_t size_;
   size_t cur_;

};

}; //namespace pf_basic

#endif //PF_BASIC_STRINGSTREAM_H_
