# File

Singleton with methods to handle files. [More...](#detailed-description)

```qml
import Script 1.0
```

## Methods

- bool **[copy](#copy)**(string fileName, string newName)
- bool **[exists](#exists)**(string fileName)
- bool **[remove](#remove)**(string fileName)
- bool **[rename](#rename)**(string oldName, string newName)
- bool **[touch](#touch)**(string fileName)
- string **[readAll](#readAll)**(string fileName)

## Detailed Description

The File singleton implements most of the static methods from \c QFile, check \c QFile
documentation.

## Method Documentation

#### <a name="copy"></a>bool **copy**(string fileName, string newName)

#### <a name="exists"></a>bool **exists**(string fileName)

#### <a name="remove"></a>bool **remove**(string fileName)

#### <a name="rename"></a>bool **rename**(string oldName, string newName)

#### <a name="touch"></a>bool **touch**(string fileName)

#### <a name="readAll"></a>string **readAll**(string fileName)