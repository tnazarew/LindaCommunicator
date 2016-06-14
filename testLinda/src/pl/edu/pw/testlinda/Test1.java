package pl.edu.pw.testlinda;

import java.util.ArrayList;

public class Test1 extends Test {

    int n;

    public Test1(int n)
    {
        this.n = n;
        this.description = "Test runs " + n + " input processes with the same pattern and "+ n+ " output processes with the matching tuple";
    }

    @Override
    public boolean run() {
        ArrayList<LindaInstanceStruct> inputList = startNInstances(n, LindaTypeEnum.INPUT, "i:1,s:\"abc\",f:*,s:>\"d\"");
        ArrayList<LindaInstanceStruct> outputList = startNInstances(n, LindaTypeEnum.OUTPUT, "1,\"abc\",3.1415,\"e\"");

        //wait for the processess to finish
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Result outputResult = checkResultsAndClean(outputList);
        Result inputResult = checkResultsAndClean(inputList, "1,\"abc\",3.1415,\"e\"");

        System.out.println("\t Output result: " + outputResult.getString());
        System.out.println("\t Input result: " + inputResult.getString());

        if (inputResult.isOutputCorrect && (outputResult.finished == n) && (inputResult.finished  == n)) {
            return true;
        } else {
            return false;
        }
    }
}
