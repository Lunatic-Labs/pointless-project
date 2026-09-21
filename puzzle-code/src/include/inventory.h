#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

// Every puzzle awards the player one useless item, which is never used for anything. The pages show
// the items collected so far, so the running gag pays off in the final inventory on the fin page.
//
// Returns the inventory to show in the sidebar of `name`'s page: a list of the item awarded by each
// puzzle before `name`. An item is handed over for solving its puzzle, so the first page shows no
// inventory (an empty string) and no page lists its own item. The items are in play order (see
// game_create_puzzles()), so adding or reordering puzzles means editing ITEMS in inventory.cpp to
// match. Throws std::runtime_error if `name` is not a puzzle that awards an item.
std::string inventory_html(const std::string &name);

#endif // INVENTORY_H
