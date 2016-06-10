package pl.edu.pw.testlinda;

import pl.edu.pw.testlinda.tests.Test1;

import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {

        //path to linda
        Constants.path = "/home/juzer/mocklinda";

        ArrayList<Test> tests = new ArrayList<>();
        tests.add(new Test1(100));

        for (Test test : tests) {
            System.out.println("Running " + test.testName);
            System.out.println(test.description);

            if (test.run()) {
                System.out.println(test.testName + " succeeded");
            } else {
                System.out.println(test.testName + " failed");
            }
        }
    }


}
