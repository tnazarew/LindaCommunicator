package pl.edu.pw.testlinda;

import pl.edu.pw.testlinda.*;
import pl.edu.pw.testlinda.LindaTypeEnum;
import pl.edu.pw.testlinda.Test;

import java.util.ArrayList;

public class Test3 extends Test {

    int n;
    int x;

    public Test3(int n, int x)
    {
        this.n = n;
        this.x = x;
        this.description = "Test runs "+ n+x +" input processes with one pattern and " + n+ " output processes with matching tuple" +
                "plus "+x+" output processes with tuple that does not match";
    }

    @Override
    public boolean run() {
        ArrayList<LindaInstanceStruct> inputList = startNInstances(n+x, LindaTypeEnum.INPUT, "i:1,s:\"abc\",f:*,s:>\"d\"");
        ArrayList<LindaInstanceStruct> matchingOutputList = startNInstances(n, LindaTypeEnum.OUTPUT, "1,\"abc\",3.1415,\"e\"");
        ArrayList<LindaInstanceStruct> unmatchedOutputList = startNInstances(x, LindaTypeEnum.OUTPUT, "1,\"ef\",3.1415,\"e\"");


        //wait for the processess to finish
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Result matchingoutputResult = checkResults(matchingOutputList);
        Result unmatchedoutputResult = checkResults(unmatchedOutputList);
        Result inputResult = checkResults(inputList);

        System.out.println("\t Matched Output result: " + matchingoutputResult.getString());
        System.out.println("\t Unmatched Output result: " + unmatchedoutputResult.getString());
        System.out.println("\t Input result: " + inputResult.getString());

        if (matchingoutputResult.finished == n && unmatchedoutputResult.finished == x
                && inputResult.finished == n && inputResult.waiting ==x) {
            return true;
        } else {
            return false;
        }
    }
}
