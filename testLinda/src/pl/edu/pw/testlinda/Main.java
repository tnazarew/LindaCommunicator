package pl.edu.pw.testlinda;

import java.util.ArrayList;

public class Main {

    public static void main(String[] args) throws InterruptedException {

        //path to linda
        Constants.path = "/home/wojtek/projekty/LindaCommunicator/main";
        Constants.msgFilePrefix = "/tmp/lin_mes";
        Constants.procFilePath = "/tmp/proc_test";
        Constants.tupleFilePath = "/tmp/tuple_test";

        ArrayList<Test> tests = new ArrayList<>();
        tests.add(new Test1(100));
        tests.add(new Test2(100));
        tests.add(new Test3(100, 100));
        tests.add(new Test4(100));
        tests.add(new Test5(100));
        tests.add(new Test6(100));

        for (Test test : tests) {
            System.out.println("----------------------------------------------------");
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
