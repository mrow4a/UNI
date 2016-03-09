# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0001_initial'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='area',
            name='id',
        ),
        migrations.AlterField(
            model_name='area',
            name='name',
            field=models.CharField(primary_key=True, db_column=b'name', default=b'World', serialize=False, max_length=256, blank=True),
        ),
    ]
