#pragma once

template<int wished_size>
struct filler
{
  static const int word_size = sizeof(int);
  static const int actual_size_in_words = (wished_size + word_size - 1) / word_size;

  int t[actual_size_in_words];
};