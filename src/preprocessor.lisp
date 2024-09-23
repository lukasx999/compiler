




(defun get-file (filename)
  (with-open-file (stream filename)
    (loop for line = (read-line stream nil) while line collect line))
  )



(defun main ()

  (setq kw_define "$define")

  (setq file (get-file "file.script"))

  (loop for line in file do
        (print line)

        (cond
          ((string-equal line kw_define) (print "define!"))
        )
  )

)

(main)
