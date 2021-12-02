package de.tobiasgrether.university.week04;

public class LevyC {

  static void levyCKurve(Canvas c, int ordnung, int length) {
    //TODO
  }

  public static void main(String[] args) {
         int ordnung = 5;
        int length = 30;
        if(args.length==2) {
            length = Integer.parseInt(args[1]);
            ordnung = Integer.parseInt(args[0]);
        }
        else if(args.length==1) {
        	ordnung = Integer.parseInt(args[0]);
        }
        else {
            System.out.println("Verwende Standardwerte: Ordnung 5, Laenge 30.");
            System.out.println("Verwendung: java LevyC Ordnung Laenge");
        }
 
        if (ordnung < 0) {
            System.out.println("Die Rekursionsordnung muss nicht-negativ sein!");
            return;
        }
        if (length < 1) {
            System.out.println("Die Laenge muss positiv sein!");
            return;
        }
        Canvas c = new Canvas();
        LevyC.levyCKurve(
            c,
            ordnung,
            length
        );
        c.refresh();
  }
}
