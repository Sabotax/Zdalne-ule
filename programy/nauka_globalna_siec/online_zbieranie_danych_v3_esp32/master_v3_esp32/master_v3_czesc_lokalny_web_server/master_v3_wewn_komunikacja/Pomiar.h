class Pomiar {
  public:
    String id;
    String waga;
    String temperatura;
    // ilosc sekund, ktora bedzie uzyta do identyfikacji czy musi nastawic sobie zegar esp-slave
    // do rozwazenia czy zawrzec tutaj jakis dodatkowy sposob identyfikacji/potwierdzenia godziny, czy tylko polegac na rtc z esp-master
    String sekundy;

    Pomiar(String _id,String _waga,String _temperatura, String _sekundy) : 
      id(_id), waga(_waga), temperatura(_temperatura),sekundy(_sekundy) {
        
    }
    Pomiar() {
      
    }
    String toString() {
      String re="";
      re += id + ",";
      re += waga + ",";
      re += temperatura;
      return re;
    }
      
};

// TODO bedzie trzeba ogarnac protokol, ze jak esp-slave sie wlaczy 1 raz to na poczatku pyta esp-master ile powinno spac za pierwszym razem zeby wyrownac cykl ze wszystkimi (np server.on mozna uzyc)
