ItemArray stack_maker(i32 capacity, Arena *arena) {
	ItemArray new_stack = {};

	new_stack.data = PushArrayZero(arena, Item, capacity);
	new_stack.capacity = capacity;
	new_stack.count = 0;

	return new_stack;
}

InventoryArray make_inventory(i32 capacity, Arena *arena) {
    InventoryArray new_inv = {};

    new_inv.data = PushArrayZero(arena, ItemArray, capacity);
    new_inv.capacity = capacity;
    new_inv.count = 0;

    return new_inv;
}

void gen_test_items(Entity *ent, Arena *arena) {
    for (int i = 0; i < ent->inventory.capacity; i++) {
    	ent->inventory.data[i] = stack_maker(99, arena);
    	ent->inventory.count++;

    	String n_str = string_alloc(string_concat(S("Item"), sprint("%d", i)));
    	String d_str = S("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

        ent->inventory.data[i].data[0].name = n_str;
        ent->inventory.data[i].data[0].type = i;
        ent->inventory.data[i].count++;
    }
}