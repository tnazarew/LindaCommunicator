package pl.edu.pw.testlinda;

import java.util.ArrayList;

public class Test5 extends Test{

    int n;

    public Test5(int n)
    {
        this.n = n;
        this.description = "Test runs " + n + " input processes with the same pattern and "+ n+ " output processes with the matching tuple";
    }

    @Override
    public boolean run() {
        ArrayList<LindaInstanceStruct> inputList = startNInstances(n, LindaTypeEnum.READ, "i:1,s:\"abc\",f:*,s:>\"d\"");
        ArrayList<LindaInstanceStruct> outputList = startNInstances(1, LindaTypeEnum.OUTPUT, "1,\"abc\",3.1415,\"e\"");

        //wait for the processess to finish
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Result outputResult = checkResults(outputList);
        Result inputResult = checkResults(inputList);

        System.out.println("\t Output result: " + outputResult.getString());
        System.out.println("\t Input result: " + inputResult.getString());

        if ((outputResult.finished == 1) && (inputResult.finished  == n)) {
            return true;
        } else {
            return false;
        }
    }
}
