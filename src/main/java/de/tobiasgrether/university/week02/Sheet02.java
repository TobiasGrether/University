package de.tobiasgrether.university.week02;

import de.tobiasgrether.university.utils.SimpleIO;

/**
 * @author lea-noora pototschnig, tobias grether
 */
public class Sheet02 {

    private static final int[] monatsDauern = new int[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    public static void main(String[] args) {
        int tage = SimpleIO.getInt("Bitte geben Sie die Tageskomponente des Startdatums ein.");
        int monat = SimpleIO.getInt("Bitte geben Sie die Monatskomponente des Startdatums ein .");
        int jahr = SimpleIO.getInt("Bitte geben Sie die Jahreskomponente des Startdatums ein .");
        int anzahlTage = SimpleIO.getInt("Bitte geben Sie die Anzahl an Tagen ein :");

        int gesamtTage = tage + anzahlTage;
        int monate = monat;

        while (gesamtTage > monatsDauern[monate - 1]) {
            gesamtTage = gesamtTage - monatsDauern[monate - 1];
            monate++;
            if (monate > 12) { // Neues Jahr beginnt
                jahr += 1;
                monate = 1;
            }
        }

        SimpleIO.output("Das Datum " + tage + "." + monat + "." + jahr + " befindet sich " + anzahlTage + " Tage nach dem Startdatum.");

    }
}
