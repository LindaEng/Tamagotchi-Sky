struct Pet {
  int exercise = 100;
  int coding = 100;
  int music = 100;

  bool alive = true;
};

Pet pet;

unsigned long lastDecay = 0;
const int decayInterval = 5000;

void decayStats() {
  pet.exercise -= random(0, 10);
  pet.coding -= random(0, 10);
  pet.music -= random(0, 10);

  if(pet.exercise <= 0 || pet.coding <= 0 || pet.music <= 0) {
    pet.alive = false;
  }
}

void doExercise() {
  pet.exercise += 10;
  Serial.print("exercise increased! stats: ");
  Serial.println(pet.exercise);
}

void doCoding() {
  pet.coding += 10;
  Serial.print("Coding increased! stats: ");
  Serial.println(pet.coding);
}

void doMusic() {
  pet.music += 10;


  Serial.print("Music increased! stats: ");
  Serial.println(pet.music);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'e') doExercise();
    if (command == 'c') doCoding();
    if (command == 'm') doMusic();
  }
  if (pet.alive == false) {
    Serial.println("Your pet is ded ;_;");
    while(true);
  }

  if (millis() - lastDecay > decayInterval) {
    decayStats();
    Serial.print("Exercise: ");
    Serial.println(pet.exercise);

    Serial.print("Coding: ");
    Serial.println(pet.coding);

    Serial.print("Music: ");
    Serial.println(pet.music);

    Serial.println("-----");

    lastDecay = millis();
  }
}
