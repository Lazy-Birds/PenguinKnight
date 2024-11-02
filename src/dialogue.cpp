void load_fairy_dialogue(Entity *fairy) {
	static String dialogue[2] = {
		S("Hey Penguin Knight Listen!"),
		S("A great adventure awaits you!"),
	};

	fairy->dialogue = dialogue;
}

void draw_dialogue_box() {
	
}