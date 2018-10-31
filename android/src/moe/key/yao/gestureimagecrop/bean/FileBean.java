package moe.key.yao.gestureimagecrop.bean;

public class FileBean {
    
    private String name;
    private String path;
    
    public FileBean(String name, String path) {
        this.name = name;
        this.path = path;
    }

    public String getName() {
        return this.name;
    }

    public String getPath() {
        return this.path;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof FileBean)) {
            return false;
        }
        FileBean that = (FileBean) o;
        return path.equals(that.path);
    }
    
    @Override
    public int hashCode() {
        return path.hashCode();
    }

}
