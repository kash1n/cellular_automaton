#ifndef TMACHINE_H
#define TMACHINE_H

#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <utility>

#define USER_NX 64
#define USER_NY 48

struct command_t
{
  int state_before;
  std::array<int, 8> states_of_neighbors;
  int state_after;
};

class cellular_space
{
public:
  cellular_space () {}
  cellular_space (int size_x, int size_y, const std::map<std::pair<int, int>, int> &states) { init (size_x, size_y, states); }
  ~cellular_space () {}
  bool init (int size_x, int size_y, const std::map<std::pair<int, int>, int> &states);

  void print ();

  int get_nx () { return m_nx; }
  int get_ny () { return m_ny; }
  int get_state (int x, int y);
  std::array<int, 8> get_neighbors_for (int x, int y);
  const std::vector<int> &get_states () { return m_states; }

  bool set_state (int x, int y, int state);

private:
  std::vector<int> m_states; // vector of states ( = positive integers)
  int m_nx;
  int m_ny;
};

class cellular_automaton
{
public:
  cellular_automaton () {}
  ~cellular_automaton () {}

  void run (bool step_by_step);
  void print_commands ();

  void add_command (const command_t command);
  bool read_commands_from_file (FILE *fp);
  bool read_states_from_file (FILE *fp);

private:
  int get_new_state (int prev_state, const std::array<int, 8> &nbrs);

  std::vector<command_t> m_commands;
  cellular_space m_space;
};

std::vector<std::string> split_string_by_spaces (const std::string &str);
std::string str_by_array (const std::array<int, 8> &array);

#endif
