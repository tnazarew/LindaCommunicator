package pl.edu.pw.testlinda;

public class LindaInstanceStruct {
    LindaTypeEnum lindaTypeEnum;
    String pattern;
    Integer ret;
    String error;
    String out;
    Process process;

    public LindaInstanceStruct(LindaTypeEnum en, String pattern)
    {
        lindaTypeEnum = en;
        this.pattern = pattern;
        ret = null;
        this.process = null;
        this.error = null;
        this.out = null;
    }
}
