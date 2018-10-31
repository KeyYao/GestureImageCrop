package moe.key.yao.gestureimagecrop.bean;

public class AlbumBean {

    private String identifier;
    private String name;
    private String url;
    
    public AlbumBean(String identifier, String name, String url) {
        this.identifier = identifier;
        this.name = (name == null ? "" : name);
        this.url = url;
    }

    public String getIdentifier() {
        return this.identifier;
    }

    public String getName() {
        return this.name;
    }

    public String getUrl() {
        return this.url;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof AlbumBean)) {
            return false;
        }
        AlbumBean that = (AlbumBean) o;
        return identifier.equals(that.identifier);
    }

    @Override
    public int hashCode() {
        return identifier.hashCode();
    }
    
    
}
