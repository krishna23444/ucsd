for $book in document("book.xml")//book[@year=2000]
      for $author in $book/author
          return $author/last
