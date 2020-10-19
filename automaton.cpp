#include <stdlib.h>
#include <unistd.h>
#include "automaton.h"
#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for

bool cellular_space::init (int size_x, int size_y, const std::map<std::pair<int, int>, int> &states)
{
  m_nx = size_x;
  m_ny = size_y;
  m_states.assign (size_x * size_y, 0);

  for (auto &map_pair : states)
    {
      int x = map_pair.first.first;
      int y = map_pair.first.second;
      int state = map_pair.second;
      if (!set_state (x, y, state))
        return false;
    }

  return true;
}

void cellular_space::print ()
{
  printf ("\n");
  for (int y = 0; y < m_ny; y++)
    {
      for (int x = 0; x < m_nx; x++)
        printf (" %d", m_states[y * m_nx + x]);
      printf ("\n");
    }
  printf ("\n");
}

int cellular_space::get_state (int x, int y)
{
  while (x < 0)
    x += m_nx;
  x = x % m_nx;
  while (y < 0)
    y += m_ny;
  y = y % m_ny;

  return m_states[y * m_nx + x];
}

std::array<int, 8> cellular_space::get_neighbors_for (int x, int y)
{
  std::array<int, 8> neighbors;
  neighbors[0] = get_state (x - 1, y - 1);
  neighbors[1] = get_state (x    , y - 1);
  neighbors[2] = get_state (x + 1, y - 1);
  neighbors[3] = get_state (x + 1, y    );
  neighbors[4] = get_state (x + 1, y + 1);
  neighbors[5] = get_state (x    , y + 1);
  neighbors[6] = get_state (x - 1, y + 1);
  neighbors[7] = get_state (x - 1, y    );

  return neighbors;
}

bool cellular_space::set_state (int x, int y, int state)
{
  if (state < 0)
    return false;
  if (x < 0 || x > m_nx)
    return false;
  if (y < 0 || y > m_ny)
    return false;
  m_states[y * m_nx + x] = state;
  return true;
}


void cellular_automaton::add_command (const command_t command)
{
  m_commands.push_back (command);
}

bool cellular_automaton::read_commands_from_file (FILE *fp)
{
  char buf[1024];
  while (fgets (buf, 1024, fp))
    {
      command_t new_command;

      std::string buf_str (buf);
      if (buf_str.empty ())
        continue;
      if (buf_str[0] == '\n')
        continue;
      if (buf_str[0] == ';')
        continue;

      auto comment_pos = buf_str.find_first_of (";");
      buf_str = buf_str.substr (0, comment_pos);

      std::vector<std::string> tokens = split_string_by_spaces (buf_str);
      if (tokens.size () < 3)
        {
          printf ("Cannot parse command \'%s\'!\n", buf);
          return false;
        }

      new_command.state_before = std::stoi (tokens[0]);

      if (tokens[1].size () != 8)
        {
          printf ("Cannot parse command \'%s\'!\n", buf);
          return false;
        }
      std::array<int, 8> son_array;
      for (int i = 0; i < 8; i++)
        son_array[i] = tokens[1][i] - '0';
      new_command.states_of_neighbors = son_array;

      auto space_pos = tokens[2].find_first_of (" ;\n");
      new_command.state_after = std::stoi (tokens[2].substr (0, space_pos));

      m_commands.push_back (new_command);
    }
  return true;
}

bool cellular_automaton::read_states_from_file (FILE *fp)
{
  std::map<std::pair<int, int>, int> states_from_file;

  char buf[1024];
  while (fgets (buf, 1024, fp))
    {
      int x, y, state;

      std::string buf_str (buf);
      if (buf_str.empty ())
        continue;
      if (buf_str[0] == '\n')
        continue;
      if (buf_str[0] == ';')
        continue;

      auto comment_pos = buf_str.find_first_of (";");
      buf_str = buf_str.substr (0, comment_pos);

      std::vector<std::string> tokens = split_string_by_spaces (buf_str);
      if (tokens.size () < 3)
        {
          printf ("Cannot parse state \'%s\'!\n", buf);
          return false;
        }
      x = std::stoi (tokens[0]);
      y = std::stoi (tokens[1]);
      state = std::stoi (tokens[2]);
      states_from_file.insert (std::pair<std::pair<int, int>, int> ({x, y}, state));
    }
  return m_space.init (USER_NX, USER_NY, states_from_file);
}

int cellular_automaton::get_new_state (int prev_state, const std::array<int, 8> &nbrs)
{
  for (command_t &cmd : m_commands)
    {
      if ((cmd.state_before == prev_state) && (cmd.states_of_neighbors == nbrs))
        return cmd.state_after;
    }
  return -1;
}

void cellular_automaton::run (bool step_by_step)
{
  while (true)
    {
      if (system ("clear") != 0)
        return;
      m_space.print();

      int nx = m_space.get_nx();
      int ny = m_space.get_ny();
      cellular_space new_space (nx, ny, {});
      for (int y = 0; y < ny; y++)
        {
          for (int x = 0; x < nx; x++)
            {
              int curr_state = m_space.get_state (x, y);
              std::array<int, 8> neighbors = m_space.get_neighbors_for (x, y);
              int new_state = get_new_state (curr_state, neighbors);
              if (new_state < 0)
                {
                  printf ("Cannot find command for state %d and neighbors %s!\n", curr_state, str_by_array(neighbors).c_str ());
                  return;
                }
              new_space.set_state (x, y, new_state);
            }
        }
      if (m_space.get_states() == new_space.get_states())
        return;
      m_space = new_space;

      if (step_by_step)
        getc (stdin);
      else
        {
          std::this_thread::sleep_for(std::chrono::microseconds (500000));
        }
    }
}

void cellular_automaton::print_commands ()
{
  printf ("My commands:\n");
  for (const command_t &comm : m_commands)
    {
      printf ("%d %s %d\n", comm.state_before,
                            str_by_array (comm.states_of_neighbors).c_str(),
                            comm.state_after);
    }
}

std::vector<std::string> split_string_by_spaces (const std::string &str)
{
  std::vector<std::string> all_words;
  int str_size = str.size ();
  std::string word;
  for (int i = 0; i < str_size; i++)
    {
      char ch = str[i];
      if (ch == ' ')
        {
          if (!word.empty ())
            {
              all_words.push_back (word);
              word.clear ();
            }
        }
      else
        word.push_back (ch);
    }
  if (!word.empty ())
    all_words.push_back (word);

  return all_words;
}

std::string str_by_array (const std::array<int, 8> &array)
{
  std::string str = "";
  for (int i = 0; i < 8; i++)
    str += std::to_string (array[i]);

  return str;
}

