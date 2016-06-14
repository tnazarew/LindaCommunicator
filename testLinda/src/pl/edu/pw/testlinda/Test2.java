package pl.edu.pw.testlinda;

import java.util.ArrayList;

public class Test2 extends Test{
    int n;

    public Test2 (int n) {
        this.n = n;
        this.description = "Test runs "+ n +" output processes with the same tuple and " + n +
                " read processes and " + n + " output processes with the matching pattern";
    }

    @Override
    public boolean run() {
        ArrayList<LindaInstanceStruct> outputList = startNInstances(n, LindaTypeEnum.OUTPUT, "1,\"abc\",3.1415,\"e\"");
        ArrayList<LindaInstanceStruct> readList = startNInstances(n, LindaTypeEnum.READ, "i:1,s:\"abc\",f:*,s:>\"d\"");
        ArrayList<LindaInstanceStruct> inputList = startNInstances(n, LindaTypeEnum.INPUT, "i:1,s:\"abc\",f:*,s:>\"d\"");

        //wait for the processess to finish
        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Result outputResult = checkResultsAndClean(outputList);
        Result readResult = checkResultsAndClean(readList, "1,\"abc\",3.1415,\"e\"");
        Result inputResult = checkResultsAndClean(inputList, "1,\"abc\",3.1415,\"e\"");

        System.out.println("\t Output result: " + outputResult.getString());
        System.out.println("\t Read result: " + readResult.getString());
        System.out.println("\t Input result: " + inputResult.getString());

        if (inputResult.isOutputCorrect && readResult.isOutputCorrect && (outputResult.finished == n) && (inputResult.finished  == n) && (readResult.error == 0)
                && (readResult.finished >= 0) && (readResult.finished <= n)) {
            return true;
        } else {
            return false;
        }
    }
}
