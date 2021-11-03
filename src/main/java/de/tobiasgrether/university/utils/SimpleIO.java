package de.tobiasgrether.university.utils;

import java.awt.GraphicsEnvironment;
import java.util.Scanner;

import javax.swing.JComboBox;
import javax.swing.JOptionPane;

/**
 * @author Thomas Stroeder, Maximilian Hippler
 * @version 24.10.2018
 * Class SimpleIO - class for input of simple input types
 * via simple dialog box (or if headless System.in use)
 * and output of strings (vie dialog box or System.out)
 */

public class SimpleIO {
    //Running without display support?
    private static final boolean HEADLESS = GraphicsEnvironment.isHeadless();

    /**
     * Gets an input object from the console. If options are supplied, the return-object ist one of options.
     * If no options are supplied, T must be a primitive type.
     *
     * @param tClass        This method returns object that are instances of this tClass
     * @param title         Title of the prompt
     * @param messages      The messages to display the user
     * @param options       The provided options, can be null, but then tClass must be primitive
     * @param initialOption The initial option (Hint for the user)
     * @param <T>           To specify return type and some parameter types
     * @return The object the user entered
     */
    private static <T> T getInputObjectConsole(Class<T> tClass, String title, Object messages[],
                                               T[] options, T initialOption) {
        Scanner scanner = new Scanner(System.in);

        System.out.println(title);

        for (Object o : messages)
            System.out.println(o.toString());

        if (options != null) {
            System.out.print("Bitte auswaehlen: |");

            for (Object option : options) {
                if (option == initialOption) {
                    System.out.print(" (INITIAL)");
                }


                System.out.print(" " + option.toString() + " |");
            }

            System.out.println("");
        }

        String result = scanner.nextLine();

        if (options != null) {
            for (int i = 0; i < options.length; i++) {
                if (options[i].toString().equals(result))
                    return options[i];
            }

            return null;
        } else {
            return parseInput(tClass, result);
        }
    }

    /**
     * Gets an input object from a JOptionPane. If options are supplied, the return-object ist one of options.
     * If no options are supplied, T must be a primitive type.
     *
     * @param tClass        This method returns object that are instances of this tClass
     * @param title         Title of the prompt
     * @param messages      The messages to display the user
     * @param options       The provided options, can be null, but then tClass must be primitive
     * @param initialOption The initial option (Hint for the user)
     * @param <T>           To specify return type and some parameter types
     * @return The object the user entered
     */
    private static <T> T getInputObjectJOptionPane(Class<T> tClass, String title, Object messages[],
                                                   T[] options, T initialOption) {
        T tmp;

        if (options == null) {
            tmp = parseInput(tClass, JOptionPane.showInputDialog(null, messages, title, JOptionPane.QUESTION_MESSAGE));
        } else {
            // 3 or less options, display with buttons
            if (options.length <= 3) {
                int result = JOptionPane.showOptionDialog(null, messages, title,
                        JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE,
                        null, options, initialOption);

                tmp = result == JOptionPane.CLOSED_OPTION ? null : options[result];
            }
            //Else display wih JComboBox
            else {
                JComboBox<T> comboBox = new JComboBox<>();

                for (T item : options) {
                    comboBox.addItem(item);
                }

                comboBox.setSelectedItem(initialOption);

                Object[] messagesObject = new Object[messages.length + 1];
                System.arraycopy(messages, 0, messagesObject, 0, messages.length);
                messagesObject[messages.length] = comboBox;

                int result = JOptionPane.showOptionDialog(null, messagesObject, title,
                        JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE,
                        null, null, null);

                tmp = result == JOptionPane.OK_OPTION ? (T) comboBox.getItemAt(comboBox.getSelectedIndex()) : null;
            }
        }

        return tmp;
    }

    /**
     * Parses the input, input must be primitive
     *
     * @param parseClass The class to return
     * @param input      The String-input to parse
     * @param <T>        The Type of class returned
     * @return The parsed input, for an Integer for example, Integer.valueOf(input) is used
     */
    private static <T> T parseInput(Class<T> parseClass, String input) {
        Object toReturn = null;

        //If user not aborted
        if (input != null) {
            //First check String
            if (parseClass.equals(String.class)) {
                toReturn = input;
            }
            //then boolean
            else if (parseClass.equals(Boolean.class)) {
                toReturn = Boolean.parseBoolean(input);
            }
            //now character
            else if (parseClass.equals(Character.class)) {
                if (input.length() != 1) {
                    throw new IllegalArgumentException("Ein Char wurde erwartet, nicht der String \"" + input + "\"");
                }

                toReturn = input.charAt(0);
            }
            //Can be an integer or ...
            else if (parseClass.equals(Integer.class)) {
                toReturn = Integer.valueOf(input);
            }
            //... a double or ...
            else if (parseClass.equals(Double.class)) {
                toReturn = Double.valueOf(input);
            }
            //a float
            else if (parseClass.equals(Float.class)) {
                toReturn = Float.valueOf(input);
            }
        }

        //Safe call, if toReturn is not set, its null. If set, each if-Statements guarantees, that it is of type T
        return parseClass.cast(toReturn);
    }

    /**
     * * String input from the user via a simple dialog.
     * * @param prompt the message string to be displayed inside dialog
     * * @return String input from the user. Null if user canceled providing input
     **/
    public static String getString(String prompt) {
        return getGenericObject(String.class, prompt, null, null);
    }

    /**
     * * char input from the user via a simple dialog.
     * * @param prompt the message string to be displayed inside dialog
     * * @return char input from the user. Character.MAX_VALUE if user canceled input.
     **/
    public static char getChar(String prompt) {
        Character c = getGenericObject(Character.class, prompt, null, null);
        return c == null ? Character.MAX_VALUE : c;
    }

    /**
     * * boolean selection from the user via a simple dialog.
     * * @param  prompt message to appear in dialog
     * * @return boolean selection from the user. False if user canceled input.
     **/
    public static boolean getBoolean(String prompt) {
        Boolean b = getGenericObject(Boolean.class, prompt, new Boolean[]{true, false}, true);
        return b == null ? false : b;
    }


    /**
     * * returns integer input from the user via a simple dialog.
     * * @param prompt the message string to be displayed inside dialog
     * * @return the input integer. Integer.MAX_VALUE if user canceled input
     **/
    public static int getInt(String prompt) {
        Integer i = getGenericObject(Integer.class, prompt, null, null);
        return i == null ? Integer.MAX_VALUE : i;
    }


    /**
     * * returns a float input from the user via a simple dialog.
     * * @param prompt the message string to be displayed inside dialog
     * * @return the input float. Float.NaN if user canceled input
     **/
    public static float getFloat(String prompt) {
        Float f = getGenericObject(Float.class, prompt, null, null);
        return f == null ? Float.NaN : f;
    }

    /**
     * * returns a double input from the user via a simple dialog.
     * * @param prompt the message string to be displayed inside dialog
     * * @return the input double. Double.NaN if user canceled input
     **/
    public static double getDouble(String prompt) {
        Double d = getGenericObject(Double.class, prompt, null, null);
        return d == null ? Double.NaN : d;
    }

    /**
     * Let the user choose between the specified options and return one of them
     *
     * @param prompt        the message string to be displayed inside dialog
     * @param options       the available options the user choose from
     * @param initialObject a hint for the user which option is default
     * @return The option, the user has chosen. Null if user canceled input.
     */
    public static Object getObject(String prompt, Object[] options, Object initialObject) {
        return getGenericObject(Object.class, prompt, options, initialObject);
    }

    /**
     * Let the user choose between the specified options and return one of them. if options are null,
     * tClass must be primitive, so that the input can be parsed
     *
     * @param tClass        The class to return
     * @param prompt        the message string to be displayed inside dialog
     * @param options       the available options the user choose from (can be null)
     * @param initialOption a hint for the user which option is default (can be null)
     * @param <T>           The Type of class returned
     * @return The option, the user has chosen. Null if user canceled input
     */
    public static <T> T getGenericObject(Class<T> tClass, String prompt,
                                         T[] options, T initialOption) {
        boolean isPrimitive = tClass == Double.class || tClass == Integer.class || tClass == Float.class
                || tClass == String.class || tClass == Character.class || tClass == Boolean.class;

        boolean hasOptions = options != null && options.length > 0;

        if (!isPrimitive && !hasOptions) {
            throw new IllegalArgumentException("Dies ist kein primitiver Datentyp, bitte spezifizieren.");
        }

        String title = "Eingabefenster";
        T toReturn = null;
        boolean success = false;
        Object[] commentArray = {prompt, "", ""};
//                 commentArray[1] = "Please enter a " + tClass.getName();
        while (!success) {
            try {
                toReturn = HEADLESS ? getInputObjectConsole(tClass, title, commentArray, options, initialOption) :
                        getInputObjectJOptionPane(tClass, title, commentArray, options, initialOption);
                success = true;
            } catch (IllegalArgumentException e) {
                //Ignore exception, prompt user again!
                commentArray[2] = "Falsche Eingabe, bitte wiederholen!";
            }
        }

        return toReturn;
    }

    /**
     * Prints the String passed as content in a window.
     *
     * @param content the displayed String
     **/
    public static void output(String content) {
        output(content, "Ausgabe");
    }

    /**
     * Prints the String passed as content in a window with the title passed as prompt.
     *
     * @param content the displayed String
     * @param prompt  the title String
     **/
    public static void output(String content, String prompt) {
        if (HEADLESS) {
            System.out.println(content);
        } else {
            JOptionPane.showMessageDialog(null, content, prompt, JOptionPane.PLAIN_MESSAGE);
        }
    }
}

