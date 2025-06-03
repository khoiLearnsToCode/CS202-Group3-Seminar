#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

// ========================================= DECLARATIONS =========================================

// DATA CLASSES DECLARATION
class Song {
public:
    string title;
    string artist;
    int length; // in seconds
    int popularity; // a score from 1 to 100

    Song(const string& t, const string& a, int l, int p);
};

// INTERFACE CLASSES DECLARATIONS
template <typename T>
class Iterator {
public:
    virtual bool hasNext() const = 0;
    virtual void first() = 0;
    virtual void next() = 0;
    virtual const T& current() const = 0;
    virtual ~Iterator() = default;
};

template <typename T>
class Container {
public:
    virtual Iterator<T>* createAlphabeticIterator() const = 0;
    virtual Iterator<T>* createLengthIterator() const = 0;
    virtual Iterator<T>* createPopularityIterator() const = 0;
    virtual ~Container() = default;
};

// FORWARD DECLARATIONS
template <typename T>
class Playlist;

// CONCRETE BASE ITERATOR CLASS DECLARATION
template <typename T>
class ConcreteIterator : public Iterator<T> {
public:
    ConcreteIterator(const Playlist<T>& playlist);
    void first() override;
    void next() override;
    bool hasNext() const override;
    const T& current() const override;
    virtual ~ConcreteIterator() = default;

protected:
    template<typename Compare>
    void sortIndices(Compare comp);
    vector<int> index;
    vector<int>::iterator myIt;
    const Playlist<T>& playlistRef;
};

// CONCRETE CLASSES DECLARATIONS
template <typename T>
class AlphabeticIterator : public ConcreteIterator<T> {
public:
    AlphabeticIterator(const Playlist<T>& playlist);
};

template <typename T>
class LengthIterator : public ConcreteIterator<T> {
public:
    LengthIterator(const Playlist<T>& playlist);
};

template <typename T>
class PopularityIterator : public ConcreteIterator<T> {
public:
    PopularityIterator(const Playlist<T>& playlist);
};

template <typename T>
class Playlist : public Container<T> {
public:        
    Playlist() = default;
    ~Playlist() override = default;
    
    void addToTheList(const T& item);
    Iterator<T>* createAlphabeticIterator() const override;
    Iterator<T>* createLengthIterator() const override;
    Iterator<T>* createPopularityIterator() const override;
        
    friend class ConcreteIterator<T>;
    friend class AlphabeticIterator<T>;
    friend class LengthIterator<T>;
    friend class PopularityIterator<T>;

private:
    vector<T> privatePlaylist;
};

// FUNCTION DECLARATIONS
void ClientCode();

// ========================================= IMPLEMENTATIONS =========================================

// SONG CLASS IMPLEMENTATION
Song::Song(const string& t, const string& a, int l, int p)
    : title(t), artist(a), length(l), popularity(p) {}

// CONCRETE ITERATOR BASE CLASS IMPLEMENTATION
template <typename T>
ConcreteIterator<T>::ConcreteIterator(const Playlist<T>& playlist) : playlistRef(playlist) {
    int tempSize = playlist.privatePlaylist.size();
    for (int i = 0; i < tempSize; i++){
        index.push_back(i);
    }
    
    myIt = index.begin();
}

template <typename T>
template<typename Compare>
void ConcreteIterator<T>::sortIndices(Compare comp) {
    sort(index.begin(), index.end(), [this, comp](int a, int b) {
        return comp(playlistRef.privatePlaylist[a], playlistRef.privatePlaylist[b]);
    });
    myIt = index.begin();
}

template <typename T>
void ConcreteIterator<T>::first() {
    myIt = index.begin();
}

template <typename T>
void ConcreteIterator<T>::next() {
    myIt++;
}

template <typename T>
bool ConcreteIterator<T>::hasNext() const {
    return (myIt != index.end());
}

template <typename T>
const T& ConcreteIterator<T>::current() const {
    if (myIt == index.end()) {
        throw out_of_range("Iterator has reached the end.");
    }
    return playlistRef.privatePlaylist[*myIt];
}

// ALPHABETIC ITERATOR IMPLEMENTATION
template <typename T>
AlphabeticIterator<T>::AlphabeticIterator(const Playlist<T>& playlist) : ConcreteIterator<T>(playlist) {
    this->sortIndices([](const T& a, const T& b) {
        return a.title < b.title;
    });
}

// LENGTH ITERATOR IMPLEMENTATION
template <typename T>
LengthIterator<T>::LengthIterator(const Playlist<T>& playlist) : ConcreteIterator<T>(playlist) {
    this->sortIndices([](const T& a, const T& b) {
        return a.length > b.length;
    });
}

// POPULARITY ITERATOR IMPLEMENTATION
template <typename T>
PopularityIterator<T>::PopularityIterator(const Playlist<T>& playlist) : ConcreteIterator<T>(playlist) {
    this->sortIndices([](const T& a, const T& b) {
        return a.popularity > b.popularity;
    });
}

// PLAYLIST CLASS IMPLEMENTATION
template <typename T>
void Playlist<T>::addToTheList(const T& item) {
    privatePlaylist.push_back(item);
}

template <typename T>
Iterator<T>* Playlist<T>::createAlphabeticIterator() const {
    return new AlphabeticIterator<T>(*this);
}

template <typename T>
Iterator<T>* Playlist<T>::createLengthIterator() const {
    return new LengthIterator<T>(*this);
}

template <typename T>
Iterator<T>* Playlist<T>::createPopularityIterator() const {
    return new PopularityIterator<T>(*this);
}

// CLIENT CODE IMPLEMENTATION
void ClientCode() {
    Playlist<Song> myFavoriteSongs;
    myFavoriteSongs.addToTheList(Song("Bohemian", "a", 354, 1000));
    myFavoriteSongs.addToTheList(Song("Stairway", "b", 482, 800));
    myFavoriteSongs.addToTheList(Song("Imagined", "c", 183, 1200));
    myFavoriteSongs.addToTheList(Song("Hey Jude", "d", 431, 900));

    Iterator<Song>* AIt = myFavoriteSongs.createAlphabeticIterator();
    Iterator<Song>* LIt = myFavoriteSongs.createLengthIterator();
    Iterator<Song>* PIt = myFavoriteSongs.createPopularityIterator();

    for (AIt->first(); AIt->hasNext(); AIt->next()) {
        cout << "Alphabetic: " << AIt->current().title << "\t\t" << AIt->current().artist << "\t\t" << AIt->current().length << "\t\t" << AIt->current().popularity << endl;
    }
    cout << endl;

    for (LIt->first(); LIt->hasNext(); LIt->next()) {
        cout << "Length: " << LIt->current().title << "\t\t" << LIt->current().artist << "\t\t" << LIt->current().length << "\t\t" << LIt->current().popularity << endl;
    }

    cout << endl;    
    
    for (PIt->first(); PIt->hasNext(); PIt->next()) {
        cout << "Popularity: " << PIt->current().title << "\t\t" << PIt->current().artist << "\t\t" << PIt->current().length << "\t\t" << PIt->current().popularity << endl;
    }

    // Clean up memory
    delete AIt;
    delete LIt;
    delete PIt;
}

int main() {
    ClientCode();
    return 0;
}