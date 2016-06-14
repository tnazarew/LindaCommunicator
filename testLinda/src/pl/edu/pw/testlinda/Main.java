package pl.edu.pw.testlinda;

import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {

        //path to linda
        Constants.path = "/home/juzer/.CLion2016.1/system/cmake/generated/LindaCommunicator-103156cf/103156cf/Debug/main";
        Constants.msgFilePrefix = "/tmp/lin_mes";
        Constants.procFilePath = "/tmp/proc_test";
        Constants.tupleFilePath = "/tmp/tuple_test";

        ArrayList<Test> tests = new ArrayList<>();
        tests.add(new Test1(10));
        tests.add(new Test2(10));
        tests.add(new Test3(10, 10));
        tests.add(new Test4(10));
        tests.add(new Test5(10));

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
